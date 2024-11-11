#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>

#define SERVER_IP "127.0.0.1"    // Change to the server's IP if not running locally
#define SERVER_PORT 8081
#define MTU_SIZE 1500             // Maximum Transmission Unit (packet size)

void send_file(int udp_sock, const char *filename, struct sockaddr_in server_addr) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        perror("File open failed");
        close(udp_sock);
        return;
    }

    char buffer[MTU_SIZE];
    int bytes_read;
    int packet_id = 0;

    printf("Starting file transmission...\n");

    // Read from file and send data in packets
    while ((bytes_read = fread(buffer, sizeof(char), MTU_SIZE, file)) > 0) {
        // Add packet ID to buffer for identification (optional)
        memcpy(buffer, &packet_id, sizeof(packet_id));

        if (sendto(udp_sock, buffer, bytes_read + sizeof(packet_id), 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
            perror("Failed to send packet");
            fclose(file);
            close(udp_sock);
            return;
        }

        packet_id++;

        // Optional: Display progress every few packets
        if (packet_id % 20 == 0) {
            printf("Sent %d packets so far...\n", packet_id);
        }
    }

    printf("File transmission completed. Total packets sent: %d\n", packet_id);

    fclose(file);
}

int main() {
    int udp_sock;
    struct sockaddr_in server_addr;

    // Create UDP socket
    udp_sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (udp_sock < 0) {
        perror("Socket creation failed");
        return 1;
    }

    // Set up server address structure
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
        perror("Invalid address/ Address not supported");
        close(udp_sock);
        return 1;
    }

    printf("Sending file to UDP server at %s:%d\n", SERVER_IP, SERVER_PORT);

    // Send file to the server
    send_file(udp_sock, "birds-chirping-75156.mp3", server_addr);  // Replace with your audio file path

    close(udp_sock);
    return 0;
}
