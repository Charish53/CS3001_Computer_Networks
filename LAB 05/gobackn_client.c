// Sender Code (Client) - go_back_n_sender.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <time.h>

#define SERVER_PORT 8080
#define SERVER_ADDR "127.0.0.1"
#define LOSS_PROBABILITY 20 // 20% packet loss probability
#define TIMEOUT 3 // Timeout in seconds
#define MAX_RETRIES 5 // Maximum number of retries for each packet

// Function prototypes
void send_packet(int sockfd, struct sockaddr_in *server_addr, int packet);
int receive_ack(int sockfd, int expected_ack);

int main() {
    int sockfd, total_packets, window_size, base = 0, next_seq_num = 0, i, ack_received;
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
    printf("Enter the window size: ");
    scanf("%d", &window_size);

    while (base < total_packets) {
        // Send packets in the window
        for (i = base; i < base + window_size && i < total_packets; i++) {
            printf("\nSending Packet %d...\n", i);
            send_packet(sockfd, &server_addr, i);
        }

        // Wait for ACKs
        int retries;
        for (i = base; i < base + window_size && i < total_packets; i++) {
            retries = 0;
            ack_received = 0;
            while (retries < MAX_RETRIES) {
                ack_received = receive_ack(sockfd, i);
                if (ack_received) {
                    printf("Acknowledgment received for Packet %d\n", i);
                    break;
                } else {
                    printf("Timeout or packet loss detected. Retransmitting Packet %d...\n", i);
                    send_packet(sockfd, &server_addr, i);
                    retries++;
                }
            }
            if (retries == MAX_RETRIES && !ack_received) {
                printf("Failed to receive acknowledgment for Packet %d after %d retries.\n", i, MAX_RETRIES);
            }
        }

        if (ack_received) {
            base += (i - base); // Slide the window
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
int receive_ack(int sockfd, int expected_ack) {
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
        int ack_num;
        if (sscanf(buffer, "ACK %d", &ack_num) == 1 && ack_num == expected_ack) {
            return 1;
        }
    }
    return 0;
}
