#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int server_socket;
    struct sockaddr_in server_addr, client_addr;
    char buffer[BUFFER_SIZE];
    char *response = "Hello from server!";
    socklen_t addr_len = sizeof(client_addr);

    // 1. Create a UDP socket
    server_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (server_socket < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // 2. Define server address and port
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // 3. Bind the socket to an IP address and port
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    printf("UDP server is up and listening on port %d...\n", PORT);

    // 4. Receive data from a client
    int bytes_received = recvfrom(server_socket, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&client_addr, &addr_len);
    if (bytes_received < 0) {
        perror("Receive failed");
        close(server_socket);
        exit(EXIT_FAILURE);
    }
    buffer[bytes_received] = '\0';  // Null-terminate the received string
    printf("Received from client: %s\n", buffer);

    // 5. Send data to the client
    if (sendto(server_socket, response, strlen(response), 0, (struct sockaddr *)&client_addr, addr_len) < 0) {
        perror("Send failed");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    printf("Response sent to client.\n");

    // 6. Close the socket
    close(server_socket);

    return 0;
}