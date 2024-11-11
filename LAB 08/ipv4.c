#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int is_valid_ip(char *ip) {
    int num, dots = 0;
    char *ptr;

    if (ip == NULL)
        return 0;

    ptr = strtok(ip, ".");
    while (ptr) {
        if (!isdigit(*ptr))  // check if the segment is numeric
            return 0;

        num = atoi(ptr);  // convert segment to integer

        if (num < 0 || num > 255)  // check if the segment is in the range 0-255
            return 0;

        ptr = strtok(NULL, ".");
        dots++;
    }
    return dots == 4;  // must have exactly 4 parts
}

char find_ip_class(int first_octet) {
    if (first_octet >= 1 && first_octet <= 126)
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
        return 'X';  // Invalid class
}

int main() {
    char ip[16];
    printf("Enter an IPv4 address: ");
    scanf("%15s", ip);

    // Make a copy of IP for validation (strtok modifies the original string)
    char ip_copy[16];
    strncpy(ip_copy, ip, 16);

    if (is_valid_ip(ip_copy)) {
        int first_octet = atoi(strtok(ip, "."));
        char ip_class = find_ip_class(first_octet);

        printf("The IP address is valid.\n");
        printf("Class of IP address: %c\n", ip_class);
    } else {
        printf("The IP address is not valid.\n");
    }

    return 0;
}
