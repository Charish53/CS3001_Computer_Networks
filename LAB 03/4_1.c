#include <stdio.h>
#include <string.h>

void addZeros(char *data, int divisorLength) {
    int length = strlen(data);
    for (int i = 0; i < divisorLength - 1; i++) {
        data[length + i] = '0';
    }
    data[length + divisorLength - 1] = '\0';
}

void performDivision(char *data, char *divisor, char *remainder) {
    int dataLen = strlen(data);
    int divisorLen = strlen(divisor);
    strcpy(remainder, data);
    for (int i = 0; i <= dataLen - divisorLen; i++) {
        if (remainder[i] == '1') {
            for (int j = 0; j < divisorLen; j++) {
                remainder[i + j] = remainder[i + j] == divisor[j] ? '0' : '1';
            }
        }
    }
}

void generateFinalMessage(char *data, char *remainder, int divisorLength) {
    int dataLen = strlen(data);
    int remainderLen = strlen(remainder);
    data[dataLen - (divisorLength - 1)] = '\0';
    strcat(data, remainder + remainderLen - (divisorLength - 1));
}

int checkError(char *remainder) {
    int remainderLen = strlen(remainder);
    for (int i = 0; i < remainderLen; i++) {
        if (remainder[i] == '1')
            return 0;
    }
    return 1;
}

int main() {
    char data[100];
    char divisor[] = "10000111";
    char crcRemainder[200];
    char receivedRemainder[200];

    while (1) {
        printf("Enter the binary data (or type 'exit' to quit): ");
        scanf("%s", data);

        if (strcmp(data, "exit") == 0) {
            break;
        }

        int divisorLength = strlen(divisor);

        addZeros(data, divisorLength);
        performDivision(data, divisor, crcRemainder);
        generateFinalMessage(data, crcRemainder, divisorLength);
        printf("Final message to be sent with CRC: %s\n", data);

        performDivision(data, divisor, receivedRemainder);
        if (checkError(receivedRemainder))
            printf("received correctly\n");
        else
            printf("error\n");
    }

    return 0;
}
