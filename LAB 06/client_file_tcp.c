#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080
#define BUFFER_SIZE 1024
#define ACK_MSG "ACK"

void send_file(FILE *fp, int sockfd);
void receive_file(int sockfd, const char *filename);
int file_exists(const char *filename);
void create_file(const char *filename);

int main() {
    int sockfd;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};
    char filename[BUFFER_SIZE] = {0};

    // Create socket
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation error");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("Invalid address or address not supported\n");
        return -1;
    }

    // Connect to the server
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection failed");
        return -1;
    }
    printf("Connected to server at %s:%d\n","127.0.0.1", PORT);
    // Step 1: Client creates a file
    printf("Enter the name of the file to create on the client: ");
    scanf("%s", filename);
    create_file(filename);

    // Step 2: Handle the server's request for a file
    recv(sockfd, buffer, BUFFER_SIZE, 0);
    printf("Server requested file: %s\n", buffer);

    if (file_exists(buffer)) {
        FILE *fp = fopen(buffer, "r");
        send_file(fp, sockfd);
        fclose(fp);
    } else {
        // Send acknowledgment if the file doesn't exist
        send(sockfd, ACK_MSG, strlen(ACK_MSG), 0);
        printf("File '%s' does not exist. Sent acknowledgment to server.\n", buffer);
    }

    // Step 3: Client requests a file from the server
    printf("Enter the name of the file to request from the server: ");
    scanf("%s", filename);
    send(sockfd, filename, strlen(filename), 0);

    // Step 4: Receive the file or ACK message
    recv(sockfd, buffer, BUFFER_SIZE, 0);
    if (strcmp(buffer, ACK_MSG) == 0) {
        printf("Server responded that the file '%s' does not exist.\n", filename);
    } else {
        FILE *fp = fopen(filename, "w");
        fprintf(fp, "%s", buffer);
        fclose(fp);
        printf("File '%s' received successfully.\n", filename);
        receive_file(sockfd, filename);
    }

    close(sockfd);
    return 0;
}

void send_file(FILE *fp, int sockfd) {
    char data[BUFFER_SIZE] = {0};

    while (fgets(data, BUFFER_SIZE, fp) != NULL) {
        if (send(sockfd, data, sizeof(data), 0) == -1) {
            perror("Failed to send file.");
            exit(EXIT_FAILURE);
        }
        memset(data, 0, BUFFER_SIZE);
    }
}

int file_exists(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file) {
        fclose(file);
        return 1;
    }
    return 0;
}

void create_file(const char *filename) {
    FILE *fp = fopen(filename, "w");
    char buffer[BUFFER_SIZE];

    if (fp == NULL) {
        perror("File creation failed");
        return;
    }

    printf("Enter contents for the file '%s':\n", filename);
    getchar();  // Clear newline from previous input
    fgets(buffer, BUFFER_SIZE, stdin);
    fprintf(fp, "%s", buffer);

    fclose(fp);
    printf("File '%s' created successfully.\n", filename);
}

void receive_file(int sockfd, const char *filename) {
    char buffer[BUFFER_SIZE] = {0};
    FILE *fp = fopen(filename, "r");

    if (fp == NULL) {
        perror("Could not open file to display");
        return;
    }

    printf("\n--- File Contents of '%s' ---\n", filename);
    while (fgets(buffer, BUFFER_SIZE, fp) != NULL) {
        printf("%s", buffer);
    }
    printf("--- End of File ---\n");

    fclose(fp);
}