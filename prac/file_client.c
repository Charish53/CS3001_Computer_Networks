#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080
#define SERVER_IP "127.0.0.1"
#define BUFFER_SIZE 1024

int main() {
    int c_soc;
    struct sockaddr_in s_addr;
    char buffer[BUFFER_SIZE] = {0};
    char filename[100];
    FILE *file;

    // Create a socket
    c_soc = socket(AF_INET, SOCK_STREAM, 0);

    // Specify server address
    s_addr.sin_family = AF_INET;
    s_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, SERVER_IP, &s_addr.sin_addr);

    // Connect to the server
    connect(c_soc, (struct sockaddr *)&s_addr, sizeof(s_addr));
    printf("Connected to server at %s:%d\n", SERVER_IP, PORT);

    // Input the filename
    printf("Enter the filename to send: ");
    scanf("%s", filename);

    // Send the filename to the server
    send(c_soc, filename, strlen(filename), 0);
    printf("Filename sent: %s\n", filename);

    // Receive acknowledgment for filename
    int bytes_received = recv(c_soc, buffer, BUFFER_SIZE, 0);
    if (bytes_received < 0) {
        perror("Error receiving acknowledgment for filename");
        exit(EXIT_FAILURE);
    }
    buffer[bytes_received] = '\0';
    printf("Server response: %s\n", buffer);

    // Open the file for reading
    file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    // Send file data to the server
    while ((bytes_received = fread(buffer, sizeof(char), BUFFER_SIZE, file)) > 0) {
        send(c_soc, buffer, bytes_received, 0);
    }

    // Close the file after sending
    fclose(file);
    printf("File sent successfully.\n");

    // Receive final response from the server
    bytes_received = recv(c_soc, buffer, BUFFER_SIZE, 0);
    buffer[bytes_received] = '\0';
    printf("Server response: %s\n", buffer);

    // Close the connection
    close(c_soc);

    return 0;
}
