#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

#define SERVER_PORT 8080
#define MTU_SIZE 1500              // Maximum packet size (MTU)
#define PACKET_LOSS_PROB 0.05     // Probability of packet loss (5%)
#define MAX_LATENCY 100           // Max simulated latency per packet in ms
#define DISPLAY_PACKET_COUNT 20   // Packet count interval for display purposes

// Structure to store each packet's latency data
typedef struct {
    int packet_id;
    int latency;
} PacketLatency;

void receive_file(int client_sock) {
    FILE *file = fopen("received_audio.mp3", "wb");
    if (!file) {
        perror("File open failed");
        close(client_sock);
        return;
    }

    char buffer[MTU_SIZE];
    int bytes_received;
    int packets_received = 0, packets_lost = 0, retransmissions = 0;
    int total_latency = 0;
    float loss_chance;
    srand(time(NULL));  // Seed for random number generator

    printf("--- Starting file reception ---\n");

    // Receive packets from client
    while ((bytes_received = recv(client_sock, buffer, MTU_SIZE, 0)) > 0) {
        packets_received++;
        loss_chance = (float)rand() / RAND_MAX;  // Simulate packet loss

        // Simulate packet loss
        if (loss_chance < PACKET_LOSS_PROB) {
            packets_lost++;
            retransmissions++;
            printf("Packet %d lost, simulating retransmission...\n", packets_received);
            continue; // Skip writing this packet to simulate loss
        }

        // Simulate latency
        int latency = rand() % MAX_LATENCY + 1;
        total_latency += latency;
        //usleep(latency * 1000);  // Convert latency to microseconds

        fwrite(buffer, sizeof(char), bytes_received, file);

        // Optional: Display every N packets received
        if (packets_received % DISPLAY_PACKET_COUNT == 0) {
            printf("Received %d packets so far...\n", packets_received);
        }
    }

    if (bytes_received < 0) {
        perror("Error receiving data");
    } else {
        printf("File received successfully.\n");
    }

    // Calculate and print summary statistics
    float average_latency = packets_received > 0 ? (float)total_latency / packets_received : 0;
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
    int server_sock, client_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size = sizeof(client_addr);

    // Create TCP socket
    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock < 0) {
        perror("Socket creation failed");
        return 1;
    }

    // Set up server address structure
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket to the specified IP and port
    if (bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(server_sock);
        return 1;
    }

    // Listen for incoming connections
    if (listen(server_sock, 1) < 0) {
        perror("Listen failed");
        close(server_sock);
        return 1;
    }
    printf("Server listening on port %d...\n", SERVER_PORT);

    // Accept a client connection
    client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &addr_size);
    if (client_sock < 0) {
        perror("Accept failed");
        close(server_sock);
        return 1;
    }
    printf("Client connected.\n");

    // Start receiving file from the client
    receive_file(client_sock);

    // Close sockets
    close(client_sock);
    close(server_sock);
    return 0;
}
