// dijkstra_client.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define INF 9999
#define N 4
#define PORT 8080

int main() {
    struct sockaddr_in server_addr;
    int graph[N][N] = {
        {0, 1, 4, INF},
        {1, 0, 2, 6},
        {4, 2, 0, 3},
        {INF, 6, 3, 0}
    };
    char buffer[1024];
    int bytes_received;

    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));
    send(client_socket, graph, sizeof(graph), 0);
    printf("Graph matrix sent to server. Waiting for shortest path results...\n");

    while ((bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0)) > 0) {
        buffer[bytes_received] = '\0';
        printf("%s", buffer);            // Print received shortest path info
        memset(buffer, 0, sizeof(buffer));
    }

    if (bytes_received < 0) {
        printf("Error in receiving data.\n");
    }

    close(client_socket);
    return 0;
}
