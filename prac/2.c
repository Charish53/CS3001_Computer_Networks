
#include <stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<sys/select.h>

#define PORT 8080
#define IP "127.0.0.1"
#define BUFFER_SIZE 1024
int main()
{
    int c;
    struct sockaddr_in s_addr;
    char buffer[BUFFER_SIZE]= {0};
    char msg[]="Hello from clienrt";
    c= socket(AF_INET,SOCK_STREAM,0);
    s_addr.sin_port=htons(PORT);
    s_addr.sin_family=AF_INET;
    inet_pton(AF_INET,IP,&s_addr.sin_addr);
    connect(c,(struct sockaddr *)&s_addr,sizeof(s_addr));
    printf("Connected to server at %s:%d\n", IP, PORT);
    send(c,msg,strlen(msg),0);
     printf("Message sent: %s\n", msg);
    int bytes_received = recv(c,buffer,BUFFER_SIZE,0);
    printf(" Message received :%s\n",buffer);
    close(c);
    

    return 0;
}