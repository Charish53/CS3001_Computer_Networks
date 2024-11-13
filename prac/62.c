// CS22B10903 ROHAN G

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

#define SERVER_IP "127.0.0.1"
#define PORT 8080
#define BUFFER_SIZE 1024
#define MAX_RETRIES 5

int main() {
    int sock;
    struct sockaddr_in server;
    char message[BUFFER_SIZE], server_reply[BUFFER_SIZE];
    int read_size;
    int retries, success;

    srand(time(NULL));

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        perror("Could not create socket");
        return 1;
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(SERVER_IP);
    server.sin_port = htons(PORT);

    if (connect(sock, (struct sockaddr*)&server, sizeof(server)) < 0) {
        perror("Connection failed");
        return 1;
    }

    for (int i = 0; i < 5; i++) {
        retries = 0;
        success = 0;
        while (retries < MAX_RETRIES && !success) {
            if (rand() % 10 < 3) {
                printf("Medium is busy! Waiting for 1 second...\n");
                sleep(1);  
                continue;
            }
            sprintf(message, "Packet %d from client", i + 1);

            if (send(sock, message, strlen(message), 0) < 0) {
                perror("Send failed");
                return 1;
            }
            printf("Sent: %s\n", message);

            if ((read_size = recv(sock, server_reply, BUFFER_SIZE, 0)) < 0) {
                perror("Receive failed");
                return 1;
            }
            server_reply[read_size] = '\0';
            printf("Received: %s\n", server_reply);

            if (strcmp(server_reply, "COLLISION") == 0) {
                retries++;
                int random_delay = (rand() % 3 + 1); 
                printf("Collision detected! Retrying in %d seconds...\n", random_delay);
                sleep(random_delay); 
            } else if (strcmp(server_reply, "ACK") == 0) {
                success = 1;  
            }
        }
        printf("Packet %d sent successfully after %d attempts.\n\n", i + 1, retries + 1);
        sleep(1);
    }

    close(sock);
    return 0;
}