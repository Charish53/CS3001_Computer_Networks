#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <ifaddrs.h>
#include <netdb.h>
#include <errno.h>
#include <time.h>

#define P 8080
#define B 1024
#define TIME 3

int main() {
    struct sockaddr_in ser;
    char buffer[B] = {0};
    char ack[B] = {0};

    srand(time(0));
    int c = socket(AF_INET, SOCK_STREAM, 0);
    struct timeval timeout;
    timeout.tv_sec = TIME;
    timeout.tv_usec = 0;
    setsockopt(c, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout));

    ser.sin_family = AF_INET;
    ser.sin_port = htons(P);
    inet_pton(AF_INET, "127.0.0.1", &ser.sin_addr);

    connect(c, (struct sockaddr*)&ser, sizeof(ser));

    int tot_pac, pac_num = 1;
    printf("Enter the total number of packets: ");
    scanf("%d", &tot_pac);

    while (pac_num <= tot_pac) {
        snprintf(buffer, B, "Packet %d", pac_num);
        printf("\nSending packet %d: %s\n", pac_num, buffer);

        send(c, buffer, strlen(buffer), 0);
        memset(ack, 0, B);

        int valread = recv(c, ack, B, 0);
        if (valread < 0) {
            printf("Timeout: No response for packet %d. Waiting for %d seconds before retrying...\n", pac_num, TIME);
        } else {
            if (strcmp(ack, "ACK") == 0) {
                printf("ACK received for packet %d. Transmission successful.\n", pac_num);
                pac_num++;
            } else if (strcmp(ack, "NACK") == 0) {
                printf("NACK received for packet %d. Retrying transmission...\n", pac_num);
            } else {
                printf("Unexpected response for packet %d. Retrying transmission...\n", pac_num);
            }
        }
    }
    printf("\nAll packets transmitted successfully.\n");
    close(c);
    return 0;
}
