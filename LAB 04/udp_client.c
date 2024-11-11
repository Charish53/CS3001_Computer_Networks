//CS22B1095
//REDDIPALLI SAI CHARISH
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int client_socket;
    struct sockaddr_in server_addr;
    char *message = "Hello from client!";
    char buffer[BUFFER_SIZE];
    socklen_t addr_len = sizeof(server_addr);

    // 1. Create a UDP socket
    client_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (client_socket < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // 2. Define server address and port
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    // Convert IPv4 address from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) <= 0) {
        perror("Invalid address / Address not supported");
        close(client_socket);
        exit(EXIT_FAILURE);
    }

    // 3. Send data to the server
    if (sendto(client_socket, message, strlen(message), 0, (struct sockaddr *)&server_addr, addr_len) < 0) {
        perror("Send failed");
        close(client_socket);
        exit(EXIT_FAILURE);
    }
    printf("Message sent to server: %s\n", message);

    // 4. Receive data from the server
    int bytes_received = recvfrom(client_socket, buffer, BUFFER_SIZE, 0, NULL, NULL);
    if (bytes_received < 0) {
        perror("Receive failed");
        close(client_socket);
        exit(EXIT_FAILURE);
    }
    buffer[bytes_received] = '\0';  // Null-terminate the received string
    printf("Received from server: %s\n", buffer);

    // 5. Close the socket
    close(client_socket);

    return 0;
}

