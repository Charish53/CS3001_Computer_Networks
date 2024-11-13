#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

#define PORT 8081
#define MAX_BUFFER 1024
#define PACKET_LOSS_PROBABILITY 30 

int simulatePacketLossOrCorruption() {
    return (rand() % 100 < PACKET_LOSS_PROBABILITY);
}

int main() {
    struct sockaddr_in ser,cli;
    socklen_t add_len= sizeof(cli);
    char buffer[MAX_BUFFER] = {0};
    char ack[] = "ACK";
    char corruptedAck[] = "CORRUPTED_ACK";
    
    srand(time(0));

    int s= socket(AF_INET, SOCK_STREAM, 0);

    ser.sin_family = AF_INET;
    ser.sin_addr.s_addr = INADDR_ANY;
    ser.sin_port = htons(PORT);
    
    bind(s, (struct sockaddr *)&ser, sizeof(ser));
    listen(s, 3);
    
    printf("Server is listening on port %d...\n", PORT);

    int c = accept(s, (struct sockaddr *)&cli, &add_len);

    int packetNumber = 1;
    while (1) {
        memset(buffer, 0, MAX_BUFFER);
        int valread = read(c, buffer, MAX_BUFFER);
        
        if (valread == 0) {
            printf("Connection closed by client.\n");
            break;
        }

        printf("\nReceived packet %d: %s\n", packetNumber, buffer);

        if (simulatePacketLossOrCorruption()) {
            if (rand() % 2 == 0) {
                printf("Packet %d lost. No ACK sent.\n", packetNumber);
            } else {
                printf("Packet %d corrupted. Sending corrupted ACK...\n", packetNumber);
                send(c, corruptedAck, strlen(corruptedAck), 0);
            }
        } else {
            printf("Packet %d received successfully. Sending ACK...\n", packetNumber);
            send(c, ack, strlen(ack), 0);
        }

        packetNumber++;
    }

    close(c);
    close(s);
    return 0;
}
