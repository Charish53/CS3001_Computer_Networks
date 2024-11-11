#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <ifaddrs.h>

void get_ip_address(char *ip) {
    char hostname[50];
    struct hostent *host_entry;

    if (gethostname(hostname, sizeof(hostname)) == -1) {
        perror("gethostname");
        exit(EXIT_FAILURE);
    }

    host_entry = gethostbyname(hostname);
    if (host_entry == NULL) {
        perror("gethostbyname");
        exit(EXIT_FAILURE);
    }

    // Copy the IP address to the provided buffer
    strcpy(ip, inet_ntoa(*((struct in_addr*)host_entry->h_addr_list[0])));
 }
// void get_ip_address(char *ip) {
//     struct ifaddrs *ifaddr, *ifa;
//     void *tmp_addr_ptr;

//     if (getifaddrs(&ifaddr) == -1) {
//         perror("getifaddrs");
//         exit(EXIT_FAILURE);
//     }

//     for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
//         if (ifa->ifa_addr == NULL) continue;

//         if (ifa->ifa_addr->sa_family == AF_INET) {  // Only IPv4
//             tmp_addr_ptr = &((struct sockaddr_in *)ifa->ifa_addr)->sin_addr;
//             inet_ntop(AF_INET, tmp_addr_ptr, ip, INET_ADDRSTRLEN);
//             break;  // Take the first non-loopback address
//         }
//     }
//     freeifaddrs(ifaddr);
// }

int main() {
    int client_socket;
    struct sockaddr_in server_addr;
    char server_reply[32];
    char client_ip[INET_ADDRSTRLEN];

    get_ip_address(client_ip);
    printf("Client IP Address: %s\n", client_ip);

    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("Could not create socket");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");  

    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        close(client_socket);
        exit(EXIT_FAILURE);
    }

    send(client_socket, client_ip, strlen(client_ip), 0);

    recv(client_socket, server_reply, sizeof(server_reply), 0);
    printf("Server reply: %s\n", server_reply);

    // Close socket
    close(client_socket);

    return 0;
}
