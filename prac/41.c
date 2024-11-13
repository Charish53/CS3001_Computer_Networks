#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<netdb.h>
#include<ifaddrs.h>
#define P 8080
char find_ip(int first_octet){
    if(first_octet>=0&&first_octet<=127)
        return 'A';
    else if(first_octet>=128&&first_octet<=191)
        return 'B';
    else if(first_octet>=192&&first_octet<=223)
        return 'C';
    else if(first_octet>=224&&first_octet<=239)
        return 'D';
    else if(first_octet>=240&&first_octet<=255)
        return 'E';
    else
        return 'X';
}
int main(){
    struct sockaddr_in ser,cli;
    socklen_t add_len= sizeof(cli);
    char ip[16];
    int s = socket(AF_INET,SOCK_STREAM,0);
    ser.sin_family= AF_INET;
    ser.sin_port=htons(P);
    ser.sin_addr.s_addr=INADDR_ANY;
    bind(s,(struct sockaddr*)&ser,sizeof(ser));
    listen(s,3);
    printf("server listening on %d \n",P);
    int c =accept(s,(struct sockaddr*)&cli,&add_len);
    recv(c,ip,sizeof(ip),0);
    int first_octet=atoi(strtok(ip,"."));
    char ip_class=find_ip(first_octet);
    char response[32];
    snprintf(response,sizeof(response),"IP:%s,CLass:%c",ip,ip_class);
    send(c,response,sizeof(response),0);
    printf("Sent to client: %s\n", response);
    close(c);
    close(s);
}