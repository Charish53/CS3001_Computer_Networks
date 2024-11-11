#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<sys/select.h>
#include<time.h>

#define PORT 8080
#define SERVER_IP "127.0.0.1"
#define BUFFER_SIZE 1024

int main(){
    int client;
    struct sockaddr_in ser_addr;
    char msg[]="Hello from Client";
    char buffer[BUFFER_SIZE] ={0};

    client = socket(AF_INET,SOCK_STREAM,0);
    ser_addr.sin_family = AF_INET;
    ser_addr.sin_port = htons(PORT);

    inet_pton(AF_INET,SERVER_IP,&ser_addr.sin_addr);

    connect(client,(struct sockaddr *)&ser_addr ,sizeof(ser_addr));
    printf("Connected to server at %s:%d\n", SERVER_IP, PORT);

    send(client,msg,strlen(msg),0);
    printf("Message sent: %s\n", msg);

    int bytes_received = recv(client , buffer ,BUFFER_SIZE,0);
    printf("Received message: %s\n", buffer);
    
    close(client);
}