#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>

#define PORT 8080
#define SERVER_IP "127.0.0.1"
#define BUFFER_SIZE 1024

int main(){
    int client_sock;
    struct sockaddr_in ser_addr;
    char buf[BUFFER_SIZE] = {0};
    char n[10], n1[10], o[2], res[BUFFER_SIZE] = {0};

    client_sock = socket(AF_INET, SOCK_STREAM, 0);
    ser_addr.sin_family = AF_INET;
    ser_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, SERVER_IP, &ser_addr.sin_addr);

    connect(client_sock, (struct sockaddr *)&ser_addr, sizeof(ser_addr));
    
    

    recv(client_sock, buf, sizeof(buf), 0);
    printf("Message from server: %s\n", buf);

    // Enter first number
    printf("Enter the first number: ");
    scanf("%s", n);
    send(client_sock, n, sizeof(n), 0);

    // Enter second number
    printf("Enter the second number: ");
    scanf("%s", n1);
    send(client_sock, n1, sizeof(n1), 0);

    // Enter operation
    printf("Enter the required operation (+, -, *, /): ");
    scanf("%s", o);
    send(client_sock, o, sizeof(o), 0);

    // Receive result
    recv(client_sock, res, sizeof(res), 0);
    printf("Result = %s\n", res);

    close(client_sock);

    return 0;
}
