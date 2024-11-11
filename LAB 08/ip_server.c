#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

char find_ip_class(int first_octet) {
    if (first_octet >= 1 && first_octet <= 127)
        return 'A';
    else if (first_octet >= 128 && first_octet <= 191)
        return 'B';
    else if (first_octet >= 192 && first_octet <= 223)
        return 'C';
    else if (first_octet >= 224 && first_octet <= 239)
        return 'D';
    else if (first_octet >= 240 && first_octet <= 255)
        return 'E';
    else
        return 'X';  // Invalid class
}

int main() {
    int server_socket, new_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);
    char client_ip[16];

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Could not create socket");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(8080);

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    listen(server_socket, 3);

    printf("Server listening on port 8080...\n");

    new_socket = accept(server_socket, (struct sockaddr *)&client_addr, &addr_len);
    if (new_socket < 0) {
        perror("Accept failed");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    recv(new_socket, client_ip, sizeof(client_ip), 0);

    int first_octet = atoi(strtok(client_ip, "."));
    char ip_class = find_ip_class(first_octet);

    char response[32];
    snprintf(response, sizeof(response), "IP: %s, Class: %c", client_ip, ip_class);
    send(new_socket, response, strlen(response), 0);

    printf("Sent to client: %s\n", response);

    close(new_socket);
    close(server_socket);

    return 0;
}
