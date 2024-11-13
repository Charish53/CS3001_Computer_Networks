
//  CS22B1093 ROHAN G

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int server_sock, client_sock;
    struct sockaddr_in server, client;
    char buffer[BUFFER_SIZE];
    int read_size;
    socklen_t client_len;

    srand(time(NULL));

    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock == -1) {
        perror("Could not create socket");
        return 1;
    }
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    if (bind(server_sock, (struct sockaddr*)&server, sizeof(server)) < 0) {
        perror("Bind failed");
        return 1;
    }

    listen(server_sock, 3);
    client_len = sizeof(client);
    client_sock = accept(server_sock, (struct sockaddr*)&client, &client_len);
    if (client_sock < 0) {
        perror("Accept failed");
        return 1;
    }

    while (1) {
        memset(buffer, 0, BUFFER_SIZE);

        read_size = recv(client_sock, buffer, BUFFER_SIZE, 0);
        if (read_size <= 0) {
            printf("Client disconnected.\n");
            break;
        }

        buffer[read_size] = '\0';
        printf("Received: %s\n", buffer);

        if (rand() % 10 < 3) {
            send(client_sock, "COLLISION", strlen("COLLISION"), 0);
            printf("COLLISION sent for %s\n", buffer );
        } else {
            send(client_sock, "ACK", strlen("ACK"), 0);
            printf("ACK sent for %s\n\n", buffer );
        }
    }

    close(client_sock);
    close(server_sock);
    return 0;
}
