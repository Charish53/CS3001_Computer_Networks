#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<netdb.h>
#include<ifaddrs.h>

#define P 8080
void get_ip(char *cli_ip){
    char host_name[50];
    struct hostent *hostentry;
    gethostname(host_name,sizeof(host_name));
    hostentry=gethostbyname(host_name);
    strcpy(cli_ip,inet_ntoa(*((struct in_addr*)hostentry->h_addr_list[0])));
}

int main(){
    struct sockaddr_in ser;
    char ser_rply[32];
    char cli_ip[INET_ADDRSTRLEN];
    get_ip(cli_ip);
    printf("Client IP Address: %s\n", cli_ip);
    int  c = socket(AF_INET,SOCK_STREAM,0);
    ser.sin_family= AF_INET;
    ser.sin_port=htons(P);
    ser.sin_addr.s_addr=inet_addr("127.0.0.1");
    connect(c,(struct sockaddr*)&ser,sizeof(ser));
    send(c,cli_ip,strlen(cli_ip),0);
    recv(c,ser_rply,sizeof(ser_rply),0);
    printf("server rply:%s\n",ser_rply);
    close(c);
}