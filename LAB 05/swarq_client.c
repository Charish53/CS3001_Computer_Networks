// Client Code (Sender) - stop_and_wait_client.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include<sys/select.h>
#include <time.h>

#define SERVER_PORT 8080
#define SERVER_ADDR "127.0.0.1"
#define TIMEOUT 3 // Timeout in seconds

// Function prototypes
void send_packet(int sockfd, struct sockaddr_in *server_addr, int packet);
int receive_ack(int sockfd);

int main() {
    int sockfd, total_packets, packet = 1, ack_received;
    struct sockaddr_in server_addr;
    socklen_t addr_len = sizeof(server_addr);

    srand(time(0)); // Seed for random number generation

    // Create UDP socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));

    // Server information
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_ADDR);

    printf("Enter the total number of packets to be transmitted: ");
    scanf("%d", &total_packets);

    while (packet <= total_packets) {
        printf("\nSending Packet %d...\n", packet);
        send_packet(sockfd, &server_addr, packet);

        ack_received = 0;

        for (int attempts = 0; attempts < TIMEOUT; attempts++) {
            ack_received = receive_ack(sockfd);
            if (ack_received) {
                break;
            } else {
                printf("Timeout or lost packet. Retransmitting Packet %d...\n", packet);
                send_packet(sockfd, &server_addr, packet);
            }
        }

        if (ack_received) {
            printf("Acknowledgment received for Packet %d\n", packet);
            packet++;
        } else {
            printf("Failed to receive acknowledgment for Packet %d\n", packet);
        }
    }

    printf("\nTransmission complete.\n");

    close(sockfd);
    return 0;
}

// Simulates sending a packet
void send_packet(int sockfd, struct sockaddr_in *server_addr, int packet) {
    char buffer[1024];
    snprintf(buffer, sizeof(buffer), "Packet %d", packet);
    sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr *)server_addr, sizeof(*server_addr));
    printf("Packet %d sent.\n", packet);
}

// Wait for acknowledgment from the server
int receive_ack(int sockfd) {
    char buffer[1024];
    struct sockaddr_in from_addr;
    socklen_t from_len = sizeof(from_addr);

    // Set socket timeout for receiving ACK
    struct timeval timeout;
    timeout.tv_sec = TIMEOUT;
    timeout.tv_usec = 0;
    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

    int n = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&from_addr, &from_len);
    if (n > 0) {
        buffer[n] = '\0';
        if (strcmp(buffer, "ACK") == 0) {
            return 1;
        }
    }
    return 0;
}
