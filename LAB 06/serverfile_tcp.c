#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<sys/select.h>
#include<time.h>

#define PORT 8080
#define BACKLOG 5
#define BUFFER_SIZE 1024

int main(){
    int server,client;
    struct sockaddr_in ser_addr,cli_addr;
    socklen_t addr_len= sizeof(cli_addr);
    char buffer[BUFFER_SIZE] ={0};
    char response[]="HELLO FROM CHARISH";

    server=socket(AF_INET,SOCK_STREAM,0);
    ser_addr.sin_port = htons(PORT);
    ser_addr.sin_family = AF_INET;
    ser_addr.sin_addr.s_addr = INADDR_ANY;

    bind(server , (struct sockaddr *)&ser_addr  , sizeof(ser_addr));
    listen(server,BACKLOG);
    printf("Server listening on port %d\n", PORT);

    client=accept(server , (struct sockaddr *)&cli_addr , &addr_len);
    printf("Connection accepted from %s:%d\n", inet_ntoa(cli_addr.sin_addr), ntohs(ser_addr.sin_port));
    
    int bytes_received= recv(client,buffer,BUFFER_SIZE,0);

    printf("Received message: %s\n", buffer);

    send(client,response,strlen(response),0);

    close(client);
    close(server);
}