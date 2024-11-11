#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080
#define BACKLOG 5
#define BUFFER_SIZE 1024

int main() {
    int s_soc, c_soc;
    struct sockaddr_in s_addr, c_addr;
    socklen_t addr_len = sizeof(c_addr);
    char buffer[BUFFER_SIZE] = {0};
    char response[] = "File received successfully!";
    FILE *file;

    // Create socket
    s_soc = socket(AF_INET, SOCK_STREAM, 0);

    // Bind IP and PORT to the socket
    s_addr.sin_family = AF_INET;
    s_addr.sin_addr.s_addr = INADDR_ANY;
    s_addr.sin_port = htons(PORT);
    bind(s_soc, (struct sockaddr *)&s_addr, sizeof(s_addr));

    // Listen for incoming connections
    listen(s_soc, BACKLOG);
    printf("Server Listening on port %d\n", PORT);

    // Accept client connection
    c_soc = accept(s_soc, (struct sockaddr *)&c_addr, &addr_len);
    printf("Connection accepted from %s:%d\n", inet_ntoa(c_addr.sin_addr), ntohs(c_addr.sin_port));

    // Receive filename from the client
    int bytes_received = recv(c_soc, buffer, BUFFER_SIZE, 0);
    if (bytes_received < 0) {
        perror("Error receiving filename");
        exit(EXIT_FAILURE);
    }
    buffer[bytes_received] = '\0';
    printf("Filename received: %s\n", buffer);

    // Create and open the file for writing
    file = fopen(buffer, "w");
    if (!file) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    // Send acknowledgment for filename
    send(c_soc, "Filename received", 17, 0);

    // Receive file data from the client and write it to the file
    while ((bytes_received = recv(c_soc, buffer, BUFFER_SIZE, 0)) > 0) {
        fwrite(buffer, sizeof(char), bytes_received, file);
        if (bytes_received < BUFFER_SIZE) {
            break; // Break if all data has been received
        }
    }

    // Close the file after receiving data
    fclose(file);
    printf("File received and saved successfully.\n");

    // Send final response to the client
    send(c_soc, response, strlen(response), 0);

    // Close the socket connections
    close(c_soc);
    close(s_soc);

    return 0;
}
