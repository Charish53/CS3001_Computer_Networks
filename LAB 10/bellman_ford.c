#include <stdio.h>
#include <stdbool.h>

#define NODES 4
#define INF 9999  // Representing infinity
void print_routing_tables(int dist[NODES][NODES]) {
    printf("\nRouting tables:\n");
    for (int i = 0; i < NODES; i++) {
        printf("Node %c: ", 'A' + i);
        for (int j = 0; j < NODES; j++) {
            if (dist[i][j] == INF) {
                printf(" INF ");
            } else {
                printf("  %d  ", dist[i][j]);
            }
        }
        printf("\n");
    }
}
void distance_vector_routing(int graph[NODES][NODES]) {
    int dist[NODES][NODES];
    bool updated;
    for (int i = 0; i < NODES; i++) {
        for (int j = 0; j < NODES; j++) {
            dist[i][j] = graph[i][j];
        }
    }
    int step = 0;
    do {
        updated = false;
        printf("\n--- Step %d ---", step++);        
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
        print_routing_tables(dist);
    } while (updated);
}
int main() {
    int graph[NODES][NODES] = {
        {0, 1, 4, INF},
        {1, 0, 2, 6},
        {4, 2, 0, 3},
        {INF, 6, 3, 0}
    };
    printf("Initial topology matrix:\n");
    print_routing_tables(graph);
    // Perform distance vector routing
    distance_vector_routing(graph);
    printf("\n--- Convergence achieved ---\n");

    return 0;
}
