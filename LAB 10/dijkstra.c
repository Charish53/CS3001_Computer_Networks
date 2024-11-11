#include <stdio.h>
#include <stdbool.h>

#define NODES 4
#define INF 9999  // Representing infinity for no direct link

void print_shortest_paths(int dist[], int prev[], int src) {
    printf("Shortest paths from node %c:\n", 'A' + src);
    for (int i = 0; i < NODES; i++) {
        if (i != src) {
            printf("To %c: Cost = %d, Path = %c", 'A' + i, dist[i], 'A' + src);
            int j = i;
            while (prev[j] != -1 && prev[j] != src) {
                printf(" -> %c", 'A' + prev[j]);
                j = prev[j];
            }
            printf(" -> %c\n", 'A' + i);
        }
    }
}

// Dijkstra's algorithm to find the shortest paths from a given source node
void dijkstra(int graph[NODES][NODES], int src) {
    int dist[NODES];      // Distance from src to each node
    bool visited[NODES];  // To check if the node is visited
    int prev[NODES]; 
    for (int i = 0; i < NODES; i++) {
        dist[i] = INF;
        visited[i] = false;
        prev[i] = -1;
    }
    dist[src] = 0;
    for (int count = 0; count < NODES - 1; count++) {
        int min = INF, u = -1;
        for (int v = 0; v < NODES; v++) {
            if (!visited[v] && dist[v] <= min) {
                min = dist[v];
                u = v;
            }
        }
        visited[u] = true;
        for (int v = 0; v < NODES; v++) {
            if (!visited[v] && graph[u][v] && dist[u] != INF &&
                dist[u] + graph[u][v] < dist[v]) {
                dist[v] = dist[u] + graph[u][v];
                prev[v] = u;
            }
        }
    }
    print_shortest_paths(dist, prev, src);
}
int main() {
    int graph[NODES][NODES] = {
        {0, 1, 4, INF},
        {1, 0, 2, 6},
        {4, 2, 0, 3},
        {INF, 6, 3, 0}
    };
    for (int i = 0; i < NODES; i++) {
        printf("\n--- Shortest paths from node %c ---\n", 'A' + i);
        dijkstra(graph, i);
    }
    return 0;
}
