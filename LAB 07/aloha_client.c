#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <time.h>

#define PORT 8080
#define MAX_MESSAGE_LEN 900
#define COLLISION_PROBABILITY 0.3 

void *send_data(void *socket_desc) {
    int sock = *(int*)socket_desc;
    char message[MAX_MESSAGE_LEN];  
    char server_reply[1024];
    int packet_number = 0;

    while (1) {
        printf("Enter a message to send (or type 'exit' to quit): ");
        fgets(message, MAX_MESSAGE_LEN, stdin);  
        if (strncmp(message, "exit", 4) == 0) {
            printf("Disconnecting...\n");
            break;
        }
        message[strcspn(message, "\n")] = '\0';
        packet_number++;
        char packet_message[1024];
        snprintf(packet_message, sizeof(packet_message), "Packet %d: %s", packet_number, message);
        double random_value = (double)rand() / RAND_MAX;
        if (random_value < COLLISION_PROBABILITY) {
            printf("Collision detected for Packet %d! Waiting to retry...\n", packet_number);
            int backoff_time = rand() % 5 + 1; 
            printf("Retrying after %d seconds...\n", backoff_time);
            sleep(backoff_time);
            printf("Resending Packet %d: %s\n", packet_number, message);
        }
        if (send(sock, packet_message, strlen(packet_message), 0) < 0) {
            puts("Send failed.");
            return 0;
        }
        if (recv(sock, server_reply, 1024, 0) < 0) {
            puts("Recv failed.");
            return 0;
        }
        printf("Server reply: %s\n", server_reply);

        memset(server_reply, 0, 1024); 
    }

    return 0;
}

int main(int argc, char *argv[]) {
    int sock;
    struct sockaddr_in server;
    pthread_t send_thread;

    srand(time(NULL)); 

    
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        printf("Could not create socket.\n");
    }
    puts("Socket created.");

    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("Connection failed.");
        return 1;
    }
    puts("Connected to server.");
    if (pthread_create(&send_thread, NULL, send_data, (void*)&sock) < 0) {
        perror("Could not create thread.");
        return 1;
    }

    pthread_join(send_thread, NULL);
    close(sock);

    return 0;
}
