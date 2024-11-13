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


#define MTU 1500
#define P 8080

int main(){
    char buffer[MTU] = {0};
    struct sockaddr_in ser,cli;
    socklen_t add_len=sizeof(cli);
    int s =socket(AF_INET,SOCK_STREAM,0);
    ser.sin_addr.s_addr=INADDR_ANY;
    ser.sin_family=AF_INET;
    ser.sin_port=htons(P);
    bind(s,(struct sockaddr*)&ser,sizeof(ser));
    listen(s,3);
    printf("server listening on %d\n",P);
    int c =accept(s,(struct sockaddr*)&cli,&add_len);
    //receive_file(c);
    int bytes_received;
    while(( bytes_received=recv(c,buffer,MTU,0)>0)){
        printf("%s",buffer);
    }
    if (bytes_received < 0) {
        perror("Error receiving data");
    } else {
        printf("\nFile received successfully.\n");
    }
    close(c);
    close(s);
}