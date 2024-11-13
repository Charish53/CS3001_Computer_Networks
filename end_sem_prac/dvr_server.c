#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define NODES 3
#define INF 9999
#define PORT 8080
int graph[NODES][NODES];
void print_routing_tables(int dist[NODES][NODES], int client_socket) {
    char message[1024];
    sprintf(message, "\nRouting tables:\n");
    send(client_socket, message, strlen(message), 0);

    for (int i = 0; i < NODES; i++) {
        sprintf(message, "Node %c: ", 'A' + i);
        send(client_socket, message, strlen(message), 0);

        for (int j = 0; j < NODES; j++) {
            if (dist[i][j] == INF) {
                sprintf(message, " INF ");
            } else {
                sprintf(message, "  %d  ", dist[i][j]);
            }
            send(client_socket, message, strlen(message), 0);
        }
        sprintf(message, "\n");
        send(client_socket, message, strlen(message), 0);
    }
}

void distance_vector_routing(int graph[NODES][NODES], int client_socket) {
    int dist[NODES][NODES];
    bool updated;
    char message[1024];

    // Copy initial graph to distance matrix
    for (int i = 0; i < NODES; i++) {
        for (int j = 0; j < NODES; j++) {
            dist[i][j] = graph[i][j];
        }
    }

    

    int step = 0;
    do {
        updated = false;
        sprintf(message, "\n--- Step %d ---", step++);
        send(client_socket, message, strlen(message), 0);

        for (int src = 0; src < NODES; src++) {
            for (int dest = 0; dest < NODES; dest++) {
                for (int via = 0; via < NODES; via++) {
                    if (graph[src][via] != INF && dist[via][dest] != INF) {
                        int new_distance = graph[src][via] + dist[via][dest];
                        if (new_distance < dist[src][dest]) {
                            dist[src][dest] = new_distance;
                            updated = true;
                        }
                    }
                }
            }
        }

        printf("Sending updated routing table to client...\n");
        print_routing_tables(dist, client_socket);

    } while (updated);

    printf("\nConvergence achieved. Final routing table sent to client.\n");
}

int main() {
    int server_socket, client_socket, c;
    struct sockaddr_in server, client;
   

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    bind(server_socket, (struct sockaddr *)&server, sizeof(server));
    listen(server_socket, 3);

    printf("Waiting for incoming connections...\n");
    socklen_t add_len = sizeof(struct sockaddr_in);

    client_socket = accept(server_socket, (struct sockaddr *)&client, (socklen_t*)&c);
    printf("Connection accepted.\n");

    // Receive the graph matrix from the client
    recv(client_socket, graph, sizeof(graph), 0);
    printf("Matrix received. Starting distance vector routing algorithm...\n");

    // Run distance vector routing algorithm
    distance_vector_routing(graph, client_socket);

    close(client_socket);
    close(server_socket);
    return 0;
}
