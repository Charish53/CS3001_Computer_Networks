#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define INF 9999
#define N 3
#define PORT 8080

int main() {
    struct sockaddr_in ser;
    int g[N][N] = {
        {0, 2, INF},
        {2, 0, 2},
        {INF, 2, 0}
    };
    char buffer[1024] = {0};
    int bytes_received;
    int c = socket(AF_INET, SOCK_STREAM, 0);
    
    ser.sin_family = AF_INET;
    ser.sin_port = htons(PORT);
    ser.sin_addr.s_addr = INADDR_ANY;

    connect(c, (struct sockaddr*)&ser, sizeof(ser));
    send(c, g, sizeof(g), 0);
    printf("Graph matrix sent to server. Waiting for routing tables...\n");

    while ((bytes_received = recv(c, buffer, sizeof(buffer) - 1, 0)) > 0) {
        buffer[bytes_received] = '\0';  // Null-terminate the received data
        printf("%s", buffer);           // Print the routing table
        memset(buffer, 0, sizeof(buffer)); // Clear buffer for the next message
    }

    if (bytes_received < 0) {
        printf("Error in receiving data.\n");
    }
    
    close(c);
    return 0;
}
