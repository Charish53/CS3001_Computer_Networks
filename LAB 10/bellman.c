#include <stdio.h>
#include <stdbool.h>

#define NODES 3
#define INF 9999  // Representing infinity

void print_routing_tables(int distance[NODES][NODES]) {
    printf("\nRouting tables:\n");
    for (int i = 0; i < NODES; i++) {
        printf("Node %c: ", 'A' + i);
        for (int j = 0; j < NODES; j++) {
            if (distance[i][j] == INF) {
                printf(" INF ");
            } else {
                printf("  %d  ", distance[i][j]);
            }
        }
        printf("\n");
    }
}

void distance_vector_routing(int graph[NODES][NODES]) {
    int distance[NODES][NODES];
    bool updated;

    // Initialize distance vector with the initial topology matrix
    for (int i = 0; i < NODES; i++) {
        for (int j = 0; j < NODES; j++) {
            distance[i][j] = graph[i][j];
        }
    }

    int step = 1;
    do {
        updated = false;
        printf("\n--- Step %d ---", step++);
        
        // Iterate over each node as a source
        for (int src = 0; src < NODES; src++) {
            // Iterate over each destination node
            for (int dest = 0; dest < NODES; dest++) {
                // Check if src has a direct link to the intermediate node `via`
                for (int via = 0; via < NODES; via++) {
                    if (graph[src][via] != INF && distance[via][dest] != INF) {
                        int new_distance = graph[src][via] + distance[via][dest];
                        if (new_distance < distance[src][dest]) {
                            distance[src][dest] = new_distance;
                            updated = true;
                        }
                    }
                }
            }
        }

        // Display the routing tables at this step
        print_routing_tables(distance);

    } while (updated);
}

int main() {
    int graph[NODES][NODES] = {
        {0, 2, 3},
        {2, 0, 1},
        {3, 1, 0},
        
    };

    printf("Initial topology matrix:\n");
    print_routing_tables(graph);

    // Perform distance vector routing
    distance_vector_routing(graph);

    printf("\n--- Convergence achieved ---\n");

    return 0;
}
