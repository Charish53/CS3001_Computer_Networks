#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<arpa/inet.h>
#include<errno.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/select.h>
#include<netdb.h>
#include<ifaddrs.h>

#define SERVER_IP "127.0.0.1"
#define P 8080
#define MTU 1500


int main(){
    char buffer[MTU]={0};
    struct sockaddr_in ser;

    int c =socket(AF_INET,SOCK_STREAM,0);
    ser.sin_family=AF_INET;
    ser.sin_port=htons(P);
    inet_pton(AF_INET,SERVER_IP,&ser.sin_addr);
    connect(c,(struct sockaddr*)&ser,sizeof(ser));
    printf("Connected to server %s on port %d.\n", SERVER_IP, P);
    //send_file(c,"1.txt");
    FILE *fp;
    char *filename="1.txt";
    fp=fopen(filename,"r");
    while (fgets(buffer, MTU, fp) != NULL) {
        send(c, buffer, strlen(buffer), 0);
    }
    fclose(fp);
    close(c);
}