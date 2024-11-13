#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define INF 9999
#define N 3
#define PORT 8080

void print_table(int dist[N][N], int c) {
    char msg[1024];
    for (int i = 0; i < N; i++) {
        sprintf(msg, "Node %c: ", 'A' + i);
        send(c, msg, strlen(msg), 0);
        for (int j = 0; j < N; j++) {
            if (dist[i][j] == INF) {
                sprintf(msg, " INF ");
            } else {
                sprintf(msg, " %d ", dist[i][j]);
            }
            send(c, msg, strlen(msg), 0);
        }
        sprintf(msg, "\n");
        send(c, msg, strlen(msg), 0);
    }
}

void bellman_ford(int g[N][N], int c) {
    char msg[1024];
    bool update;
    int dist[N][N];
    
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            dist[i][j] = g[i][j];
        }
    }
    
    int step = 0;
    do {
        update = false;
        sprintf(msg, "\n=== Step %d ===\n", step++);
        send(c, msg, strlen(msg), 0);
        
        for (int src = 0; src < N; src++) {
            for (int dest = 0; dest < N; dest++) {
                for (int via = 0; via < N; via++) {
                    if (g[src][via] != INF && dist[via][dest] != INF) {
                        int new_dist = g[src][via] + dist[via][dest];
                        if (new_dist < dist[src][dest]) {
                            dist[src][dest] = new_dist;
                            update = true;
                        }
                    }
                }
            }
        }
        
        print_table(dist, c);
    } while (update);
    
    printf("\nConvergence achieved\n"); 
}

int main() {
    int g[N][N];
    struct sockaddr_in ser, cli;
    socklen_t add_len = sizeof(cli);
    int s = socket(AF_INET, SOCK_STREAM, 0);
    ser.sin_family = AF_INET;
    ser.sin_port = htons(PORT);
    ser.sin_addr.s_addr = INADDR_ANY;
      
    bind(s, (struct sockaddr*)&ser, sizeof(ser));
    listen(s, 5);

    printf("Server listening on %d\n", PORT);
    
    int c = accept(s, (struct sockaddr*)&cli, &add_len); // Corrected to use 's' instead of 'c'
    recv(c, g, sizeof(g), 0);
    printf("Matrix received. Starting distance vector routing algorithm...\n");
    
    bellman_ford(g, c);
    
    close(c);
    close(s);
    return 0;
}
