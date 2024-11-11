#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

#define PORT 8080
#define MAX_MESSAGE_LEN 1024
#define MEDIUM_CHECK_PROBABILITY 0.7  

int main() {
    int sock;
    struct sockaddr_in server;
    char message[MAX_MESSAGE_LEN], server_reply[MAX_MESSAGE_LEN];    
    srand(time(0));
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        printf("Could not create socket.\n");
        return 1;
    }    
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("Connection failed.");
        return 1;
    }
    printf("Connected to server.\n");

    while (1) {
        printf("Enter message to send (type 'exit' to quit): ");
        fgets(message, MAX_MESSAGE_LEN, stdin);

        if (strncmp(message, "exit", 4) == 0) {
            printf("Disconnecting...\n");
            break;
        }
        float random_value = (float)rand() / RAND_MAX;
        if (random_value < MEDIUM_CHECK_PROBABILITY) {
            printf("Medium perceived as free (probability check passed).\n");
            if (send(sock, message, strlen(message), 0) < 0) {
                puts("Send failed.");
                return 1;
            }
            if (recv(sock, server_reply, MAX_MESSAGE_LEN, 0) < 0) {
                puts("Receive failed.");
                break;
            }
            printf("Server: %s", server_reply);
            if (strstr(server_reply, "Collision detected")) {
                int backoff_time = rand() % 5 + 1;  
                printf("Backing off for %d seconds...\n", backoff_time);
                sleep(backoff_time);  
            }
        } else {
            printf("Medium perceived as busy (probability check failed). Retrying...\n");
            int retry_time = rand() % 3 + 1; 
            sleep(retry_time);  
        }
    }

    close(sock);
    return 0;
}
