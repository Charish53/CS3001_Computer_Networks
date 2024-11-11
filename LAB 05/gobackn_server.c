
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

#define SERVER_PORT 8080
#define LOSS_PROBABILITY 20 // 20% packet loss probability

// Function prototypes
int simulate_packet_loss();
void send_ack(int sockfd, struct sockaddr_in *client_addr, int ack_num);

int main() {
    int sockfd;
    char buffer[1024];
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);
    int expected_packet = 0;

    srand(time(0)); // Seed for random number generation

    // Create UDP socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    memset(&client_addr, 0, sizeof(client_addr));

    // Server information
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket with the server address
    if (bind(sockfd, (const struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on port %d...\n", SERVER_PORT);

    while (1) {
        int n = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&client_addr, &addr_len);
        buffer[n] = '\0';

        int received_packet;
        if (sscanf(buffer, "Packet %d", &received_packet) == 1) {
            printf("\nReceived: %s\n", buffer);

            // Simulate packet loss
            if (simulate_packet_loss()) {
                printf("Simulated packet loss for %s. Not sending ACK.\n", buffer);
            } else {
                if (received_packet == expected_packet) {
                    // Send cumulative ACK
                    send_ack(sockfd, &client_addr, expected_packet);
                    expected_packet++;
                } else {
                    // Send cumulative ACK for the last correctly received packet
                    send_ack(sockfd, &client_addr, expected_packet - 1);
                }
            }
        }
    }

    close(sockfd);
    return 0;
}

// Simulates packet loss with a 20% chance
int simulate_packet_loss() {
    int random_value = rand() % 100;
    return random_value < LOSS_PROBABILITY;
}

// Simulates sending an acknowledgment (ACK)
void send_ack(int sockfd, struct sockaddr_in *client_addr, int ack_num) {
    char ack[50];
    snprintf(ack, sizeof(ack), "ACK %d", ack_num);
    sendto(sockfd, ack, strlen(ack), 0, (struct sockaddr *)client_addr, sizeof(*client_addr));
    printf("ACK %d sent.\n", ack_num);
}
