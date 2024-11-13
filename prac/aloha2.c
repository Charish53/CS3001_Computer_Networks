#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <time.h>

#define PORT 8081
#define MAX_BUFFER 1024
#define TIMEOUT 3  

int main() {
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char buffer[MAX_BUFFER] = {0};
    char ack[MAX_BUFFER] = {0};
    int totalPackets, packetNumber = 1;
    
    srand(time(0));

    sock = socket(AF_INET, SOCK_STREAM, 0);

    struct timeval timeout;
    timeout.tv_sec = TIMEOUT;
    timeout.tv_usec = 0;
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

    connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    printf("Enter the total number of packets to be transmitted: ");
    scanf("%d", &totalPackets);

    while (packetNumber <= totalPackets) {
        snprintf(buffer, MAX_BUFFER, "Packet %d", packetNumber);
        printf("\nSending packet %d: %s\n", packetNumber, buffer);
        
        send(sock, buffer, strlen(buffer), 0);

        memset(ack, 0, MAX_BUFFER);
        valread = recv(sock, ack, MAX_BUFFER, 0);

        if (valread < 0) {
            printf("Collision detected for packet %d. Waiting for 3 time units and sending again\n", packetNumber);
        } else {
            if (strcmp(ack, "ACK") == 0) {
                printf("ACK received for packet %d. No collision detected.\n", packetNumber);
                packetNumber++;
            } else if (strcmp(ack, "CORRUPTED_ACK") == 0) {
                printf("Collision detected for packet %d. Retransmitting...\n", packetNumber);
            } else {
                printf("Incorrect ACK received for packet %d. Retransmitting...\n", packetNumber);
            }
        }
    }

    printf("\nAll packets transmitted successfully.\n");
    close(sock);
    return 0;
}
