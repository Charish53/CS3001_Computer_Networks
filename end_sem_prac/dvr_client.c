#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080
#define INF 9999
int main() {
    int client_socket;
    struct sockaddr_in server;
    int graph[3][3] = {
        {0, 2, INF},
        {2, 0, 2},
        {INF, 2, 0}
    };
    char buffer[1024] = {0};
    int bytes_received;

    // Create socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);

    // Define server address
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = INADDR_ANY;

    // Connect to server
    if (connect(client_socket, (struct sockaddr *)&server, sizeof(server)) < 0) {
        printf("Connection failed.\n");
        return 1;
    }

    // Send the graph matrix to the server
    send(client_socket, graph, sizeof(graph), 0);
    printf("Graph matrix sent to server. Waiting for routing tables...\n");

    // Receive the routing tables from the server
    while ((bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0)) > 0) {
        buffer[bytes_received] = '\0';  // Null-terminate the received data
        printf("%s", buffer);            // Print the routing table
        memset(buffer, 0, sizeof(buffer)); // Clear buffer for the next message
    }

    if (bytes_received < 0) {
        printf("Error in receiving data.\n");
    }

    // Close the socket
    close(client_socket);
    return 0;
}
