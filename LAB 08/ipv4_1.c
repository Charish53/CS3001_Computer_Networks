#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char find_ip_class(int first_octet) {
    if (first_octet >= 0 && first_octet <= 127)
        return 'A';
    else if (first_octet >= 128 && first_octet <= 191)
        return 'B';
    else if (first_octet >= 192 && first_octet <= 223)
        return 'C';
    else if (first_octet >= 224 && first_octet <= 239)
        return 'D';
    else if (first_octet >= 240 && first_octet <= 255)
        return 'E';
    else
        return 'X'; 
}
int main() {
    char ip[16];
    printf("Enter an IPv4 address: ");
    scanf("%15s", ip);

    int num, dots = 0;
    char *ptr = strtok(ip, ".");

    while (ptr != NULL) {
        // for (int i = 0; i < (int)strlen(ptr); i++) {
        //     if (!isdigit(ptr[i])) {  
        //         printf("The IP address is not valid.\n");
        //         return 0;
        //     }
        // }
        for (size_t i = 0; i < strlen(ptr); i++) {
            if (!isdigit(ptr[i])) {  
                printf("The IP address is not valid.\n");
                return 0;
            }
        }

        num = atoi(ptr);
        if (num < 0 || num > 255) {  
            printf("The IP address is not valid.\n");
            return 0;
        }

        ptr = strtok(NULL, ".");
        dots++;
    }

    if (dots != 4) {  
        printf("The IP address is not valid.\n");
        return 0;
    }

    int first_octet = atoi(ip);
    char ip_class = find_ip_class(first_octet);

    printf("The IP address is valid.\n");
    printf("Class of IP address: %c\n", ip_class);

    return 0;
}
