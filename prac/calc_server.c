#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>

#define PORT 8080
#define BACKLOG 5
#define BUFFER_SIZE 1024 
float perform_calculation(float num1, float num2, char operator) {
    switch (operator) {
        case '+': return num1 + num2;
        case '-': return num1 - num2;
        case '*': return num1 * num2;
        case '/': return num1 / num2;
        default: return 0;
    }
}

int main(){
    int server_sock, client_sock;
    struct sockaddr_in ser_addr, cli_addr;
    socklen_t addr_len = sizeof(cli_addr);
    char buf[] = "HELLO FROM SERVER";
    char n[10], n1[10], o[2], result[BUFFER_SIZE] = {0};
    float num1, num2, calc_result;
    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    ser_addr.sin_family = AF_INET;
    ser_addr.sin_port = htons(PORT);
    ser_addr.sin_addr.s_addr = INADDR_ANY;
    bind(server_sock, (struct sockaddr *)&ser_addr, sizeof(ser_addr));
    listen(server_sock, BACKLOG);
    printf("Server listening on port %d\n", PORT);
    client_sock = accept(server_sock, (struct sockaddr *)&cli_addr, &addr_len);
    printf("Connection accepted from %s:%d\n", inet_ntoa(cli_addr.sin_addr), ntohs(ser_addr.sin_port));
send(client_sock, buf, sizeof(buf), 0);
    // Receive the first number
    recv(client_sock, n, sizeof(n), 0);
    num1 = atof(n);  // Convert string to float
// Receive the second number
recv(client_sock, n1, sizeof(n1), 0);
    num2 = atof(n1);  // Convert string to float
    // Receive the operation
    recv(client_sock, o, sizeof(o), 0);
    // Perform the calculation
    calc_result = perform_calculation(num1, num2, o[0]);
// Prepare the result as a string
    snprintf(result, sizeof(result), "%.2f", calc_result);
// Send the result to the client
    send(client_sock, result, sizeof(result), 0);

    close(client_sock);
    close(server_sock);

    return 0;
}
