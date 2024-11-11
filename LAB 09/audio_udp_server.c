#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include<sys/time.h>
#include <arpa/inet.h>
#include <errno.h>
#include <time.h>

#define SERVER_PORT 8081
#define MTU_SIZE 1500             
#define PACKET_LOSS_PROB 0.05    

typedef struct {
    int packet_id;
    int latency;
} PacketLatency;

void receive_file(int udp_sock, int total_packets) {
    FILE *file = fopen("received_audio_udp.mp3", "wb");
    if (!file) {
        perror("File open failed");
        close(udp_sock);
        return;
    }

    struct sockaddr_in client_addr;
    socklen_t addr_size = sizeof(client_addr);
    char buffer[MTU_SIZE];
    int bytes_received;
    int packets_received = 0, packets_lost = 0, retransmissions = 0;
    int total_latency = 0;

    srand(time(NULL));

    printf("Waiting for UDP packets...\n");

    // Set socket timeout
    struct timeval timeout;
    timeout.tv_sec = 5;  // Wait for 5 seconds of inactivity before timing out
    timeout.tv_usec = 0;
    setsockopt(udp_sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

    while ((bytes_received = recvfrom(udp_sock, buffer, MTU_SIZE, 0, (struct sockaddr *)&client_addr, &addr_size)) > 0) {
        packets_received++;
        printf("Received packet %d with %d bytes\n", packets_received, bytes_received);

        float loss_chance = (float)rand() / RAND_MAX;

        if (loss_chance < PACKET_LOSS_PROB) {
            printf("Simulated packet loss for packet %d\n", packets_received);
            packets_lost++;
            retransmissions++;
            continue;
        }

        int latency = rand() % 100 + 1;  
        total_latency += latency;

        fwrite(buffer, sizeof(char), bytes_received, file);
    }

    if (bytes_received < 0 && errno == EWOULDBLOCK) {
        printf("Timeout reached. No more packets are being received.\n");
    }

    // Calculate average latency
    float average_latency = packets_received > 0 ? (float)total_latency / packets_received : 0;

    // Print the summary
    printf("\n--- Transmission Summary ---\n");
    printf("Total Packets Received: %d\n", packets_received);
    printf("Total Packets Lost: %d\n", packets_lost);
    printf("Total Retransmissions: %d\n", retransmissions);
    printf("Average Latency per Packet: %.2f ms\n", average_latency);
    printf("Total Transmission Latency: %d ms\n", total_latency);
    printf("Efficiency : %.2f%%\n",(float)(packets_received-packets_lost)/packets_received*100);
    printf("Packet Loss Rate: %.2f%%\n", (float)packets_lost / packets_received * 100);
    printf("Reassemble reliability: %.2f%%\n", (float)(packets_received-packets_lost)/packets_received*100);

    fclose(file);
}

int main() {
    int udp_sock;
    struct sockaddr_in server_addr;

    udp_sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (udp_sock < 0) {
        perror("Socket creation failed");
        return 1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(udp_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(udp_sock);
        return 1;
    }

    printf("UDP Server listening on port %d...\n", SERVER_PORT);

    int total_packets = 1000; 
    receive_file(udp_sock, total_packets);

    close(udp_sock);
    return 0;
}
