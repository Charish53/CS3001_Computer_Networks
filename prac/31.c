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
#define LOSS 30
#define B 1024

int simul_loss() {
    return (rand() % 100 < LOSS);
}

int main() {
    struct sockaddr_in ser, cli;
    socklen_t add_len = sizeof(cli);
    char buffer[B] = {0};
    char ack[] = "ACK";
    char nack[] = "NACK";
    srand(time(0));
    int s = socket(AF_INET, SOCK_STREAM, 0);
    ser.sin_family = AF_INET;
    ser.sin_port = htons(P);
    ser.sin_addr.s_addr = INADDR_ANY;
    bind(s, (struct sockaddr*)&ser, sizeof(ser));
    listen(s, 5);
    printf("Server listening on port %d...\n", P);
    int c = accept(s, (struct sockaddr*)&cli, &add_len);
    int pac_num = 1;
    while (1) {
        memset(buffer, 0, B);
        int valread = read(c, buffer, B);
        if (valread == 0) {
            printf("Connection closed by client.\n");
            break;
        }
        printf("Received packet %d: %s\n", pac_num, buffer);
        if (simul_loss()) {
            if (rand() % 2 == 0) {
                printf("Packet %d lost. No ACK sent.\n", pac_num);
            } else {
                printf("Packet %d corrupted. Sending NACK...\n", pac_num);
                send(c, nack, sizeof(nack), 0);
            }
        } else {
            printf("Packet %d received successfully. Sending ACK...\n", pac_num);
            send(c, ack, sizeof(ack), 0);
        }
        pac_num++;
    }
    close(c);
    close(s);
    return 0;
}
