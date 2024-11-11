#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <time.h>

#define PORT 8080
#define MAX_USERS 5

int user_count = 0;
pthread_mutex_t medium_lock;

void *handle_user(void *socket_desc) {
    int new_socket = *(int*)socket_desc;
    char message[1024];
    int read_size;
    int packet_number;

    while ((read_size = recv(new_socket, message, 1024, 0)) > 0) {
        pthread_mutex_lock(&medium_lock);
        printf("Received packet: %s", message);
        sscanf(message, "Packet %d", &packet_number);
        int collision = rand() % 2;
        if (collision == 0) {
            printf("\nCollision detected on Packet %d! Notifying user to retry...\n", packet_number);
            char collision_message[50];
            sprintf(collision_message, "Collision occurred on Packet %d. Please resend.\n", packet_number);
            send(new_socket, collision_message, strlen(collision_message), 0);
        } else {
            printf("\nPacket %d received successfully.\n", packet_number);
            char success_message[50];
            sprintf(success_message, "Packet %d received successfully.\n", packet_number);
            send(new_socket, success_message, strlen(success_message), 0);
        }
        pthread_mutex_unlock(&medium_lock);
        memset(message, 0, 1024);
    }

    if (read_size == 0) {
        printf("User disconnected.\n");
        fflush(stdout);
    } else if (read_size == -1) {
        perror("recv failed");
    }
    free(socket_desc);
    pthread_mutex_lock(&medium_lock);
    user_count--;
    pthread_mutex_unlock(&medium_lock);

    return 0;
}

int main(int argc, char *argv[]) {
    int server_socket, client_socket, c, *new_sock;
    struct sockaddr_in server, client;
    
    srand(time(NULL)); 
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        printf("Could not create socket.\n");
    }
    puts("Socket created.");
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);
    if (bind(server_socket, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("Bind failed.");
        return 1;
    }
    puts("Bind done.");
    listen(server_socket, 3);
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
    pthread_mutex_init(&medium_lock, NULL);

    while ((client_socket = accept(server_socket, (struct sockaddr *)&client, (socklen_t*)&c))) {
        puts("Connection accepted.");
        pthread_mutex_lock(&medium_lock);
        if (user_count >= MAX_USERS) {
            printf("Max user limit reached, rejecting connection.\n");
            close(client_socket);
            pthread_mutex_unlock(&medium_lock);
            continue;
        }
        user_count++;
        pthread_mutex_unlock(&medium_lock);
        pthread_t user_thread;
        new_sock = malloc(1);
        *new_sock = client_socket;

        if (pthread_create(&user_thread, NULL, handle_user, (void*)new_sock) < 0) {
            perror("Could not create thread.");
            return 1;
        }
        pthread_detach(user_thread);
    }

    if (client_socket < 0) {
        perror("Accept failed.");
        return 1;
    }

    pthread_mutex_destroy(&medium_lock);
    return 0;
}
