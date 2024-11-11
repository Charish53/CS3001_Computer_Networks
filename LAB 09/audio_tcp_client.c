#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>

#define SERVER_IP "127.0.0.1"    // Change to server's IP if not running locally
#define SERVER_PORT 8080
#define MTU_SIZE 1500             // Maximum Transmission Unit (packet size)

void send_file(int server_sock, const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        perror("File open failed");
        close(server_sock);
        return;
    }

    char buffer[MTU_SIZE];
    int bytes_read;
    int total_packets = 0;

    printf("Starting file transmission...\n");

    // Read from file and send data in chunks
    while ((bytes_read = fread(buffer, sizeof(char), MTU_SIZE, file)) > 0) {
        if (send(server_sock, buffer, bytes_read, 0) < 0) {
            perror("Failed to send packet");
            fclose(file);
            close(server_sock);
            return;
        }
        total_packets++;
        
        // Optional: Display progress every few packets
        if (total_packets % 20 == 0) {
            printf("Sent %d packets so far...\n", total_packets);
        }
    }

    printf("File transmission completed. Total packets sent: %d\n", total_packets);

    fclose(file);
}

int main() {
    int client_sock;
    struct sockaddr_in server_addr;

    // Create TCP socket
    client_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (client_sock < 0) {
        perror("Socket creation failed");
        return 1;
    }

    // Set up server address structure
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
        perror("Invalid address/ Address not supported");
        close(client_sock);
        return 1;
    }

    // Connect to the server
    if (connect(client_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection to server failed");
        close(client_sock);
        return 1;
    }
    printf("Connected to server %s on port %d.\n", SERVER_IP, SERVER_PORT);

    // Send file to the server
    send_file(client_sock, "birds-chirping-75156.mp3");  // Replace with your audio file path

    close(client_sock);
    return 0;
}
