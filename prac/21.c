// dijkstra_server.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define INF 9999
#define N 4
#define PORT 8080

void dijkstra(int graph[N][N], int src, int dist[N], int prev[N]) {
    bool visited[N] = {false};
    for (int i = 0; i < N; i++) {
        dist[i] = INF;
        visited[i] = false;
        prev[i] = -1;
    }
    dist[src] = 0;

    for (int count = 0; count < N - 1; count++) {
        int min = INF, u = -1;
        for (int v = 0; v < N; v++) {
            if (!visited[v] && dist[v] <= min) {
                min = dist[v];
                u = v;
            }
        }
        visited[u] = true;

        for (int v = 0; v < N; v++) {
            if (!visited[v] && graph[u][v] && dist[u] != INF &&
                dist[u] + graph[u][v] < dist[v]) {
                dist[v] = dist[u] + graph[u][v];
                prev[v] = u;
            }
        }
    }
}

void print_shortest_paths(int dist[N], int prev[N], int src, int client_socket) {
    char msg[1024];
    sprintf(msg, "\nShortest paths from node %c:\n", 'A' + src);
    send(client_socket, msg, strlen(msg), 0);

    for (int i = 0; i < N; i++) {
        if (i != src) {
            sprintf(msg, "To %c: Cost = %d, Path = %c", 'A' + i, dist[i], 'A' + src);
            send(client_socket, msg, strlen(msg), 0);
            int j = i;
            while (prev[j] != -1 && prev[j] != src) {
                sprintf(msg, " -> %c", 'A' + prev[j]);  
                send(client_socket, msg, strlen(msg), 0);
                j = prev[j];
            }
            sprintf(msg, " -> %c\n", 'A' + i);
            send(client_socket, msg, strlen(msg), 0);
        }
    }
}

int main() {
    int graph[N][N];
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);  
    server_addr.sin_addr.s_addr = INADDR_ANY;

    bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));
    listen(server_socket, 5);
    printf("Server listening on port %d\n", PORT);

    int client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &addr_len);
    recv(client_socket, graph, sizeof(graph), 0);

    for (int src = 0; src < N; src++) {
        int dist[N], prev[N];
        dijkstra(graph, src, dist, prev);
        print_shortest_paths(dist, prev, src, client_socket);
    }

    close(client_socket);
    close(server_socket);
    return 0;
}
