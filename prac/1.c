#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<sys/select.h>
#include<unistd.h>

#define PORT 8080
#define BACKLOG 5
#define BUFFER_SIZE 1024

int main(){ 
    struct sockaddr_in s_addr,c_addr;
    socklen_t add_len=sizeof(c_addr);
    char buffer[BUFFER_SIZE]={0};
    char response[]="Hello from server!";
    
    int s=socket(AF_INET,SOCK_STREAM,0);
    s_addr.sin_port=htons(PORT);
    s_addr.sin_family= AF_INET;
    s_addr.sin_addr.s_addr = INADDR_ANY;
    
    bind(s,(struct sockaddr *)&s_addr,sizeof(s_addr));
    listen(s,BACKLOG);
    //print listening on PORT
    printf("Server listening on %d\n",PORT);
    int c=accept(s,(struct sockaddr *)&c_addr, &add_len);
    int bytes_received= recv(c,buffer,BUFFER_SIZE,0);
    send(c,response,strlen(response),0);
    
    close(c);
    close(s);
}