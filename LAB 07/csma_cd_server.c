#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <time.h>

#define PORT 8080
#define MAX_CLIENTS 5
#define MEDIUM_BUSY 1
#define MEDIUM_FREE 0

int medium_status = MEDIUM_FREE;
pthread_mutex_t medium_mutex;  
int connected_clients = 0;

void *handle_client(void *socket_desc) {
    int client_sock = *(int*)socket_desc;
    int collision_detected = 0;

    char client_message[1024];
    char response[1024];

    while (1) {
        memset(client_message, 0, 1024);
        if (recv(client_sock, client_message, sizeof(client_message), 0) > 0) {
            pthread_mutex_lock(&medium_mutex); 

            if (medium_status == MEDIUM_FREE) {
                printf("Medium is free. Client can send data.\n");
                medium_status = MEDIUM_BUSY;
                printf("Client is transmitting: %s", client_message);
                sleep(1);
                if (collision_detected) {
                    snprintf(response, sizeof(response), "Collision detected! Stop transmission.\n");
                    collision_detected = 0;  
                } else {
                    
                    snprintf(response, sizeof(response), "Data transmitted successfully: %.900s", client_message);

                }
                medium_status = MEDIUM_FREE; 
            } else {
                snprintf(response, sizeof(response), "Medium busy, wait for retry.\n");
                collision_detected = 1;
            }

            pthread_mutex_unlock(&medium_mutex);
            send(client_sock, response, strlen(response), 0);
        }
    }
    close(client_sock);
    free(socket_desc);
    connected_clients--;
    return 0;
}

int main() {
    int server_sock, client_sock, c;
    struct sockaddr_in server, client;
    pthread_t client_thread;
    pthread_mutex_init(&medium_mutex, NULL);
    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock == -1) {
        printf("Could not create socket.\n");
        return 1;
    }
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);
    if (bind(server_sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("Bind failed.");
        return 1;
    }
    listen(server_sock, MAX_CLIENTS);
    printf("Server is listening on port %d...\n", PORT);
    c = sizeof(struct sockaddr_in);
    while ((client_sock = accept(server_sock, (struct sockaddr *)&client, (socklen_t*)&c)) && connected_clients < MAX_CLIENTS) {
        printf("Client connected.\n");
        pthread_t client_thread;
        int *new_sock = malloc(1);
        *new_sock = client_sock;

        if (pthread_create(&client_thread, NULL, handle_client, (void*)new_sock) < 0) {
            perror("Could not create thread.");
            return 1;
        }

        connected_clients++;
    }

    if (client_sock < 0) {
        perror("Accept failed.");
        return 1;
    }

    pthread_mutex_destroy(&medium_mutex);
    close(server_sock);

    return 0;
}
