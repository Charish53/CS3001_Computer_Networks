//CS22B1095
//REDDIPALLI SAI CHARISH
#include <stdio.h>
#include <string.h>

void bitStuffing(char *inputMsg, char *stuffedMsg) {
    int index = 0, oneCount = 0, msgLength;
    msgLength = strlen(inputMsg);
    for (index = 0; index < msgLength; index++) {
        if (oneCount == 5) {
            strcat(stuffedMsg, "0");
            oneCount = 0;
        }
        if (inputMsg[index] == '1') {
            oneCount++;
        } else {
            oneCount = 0;
        }
        strncat(stuffedMsg, &inputMsg[index], 1);
    }
    if (oneCount == 5) {
        strcat(stuffedMsg, "0");
    }
}

void reverseBitStuffing(char *stuffedMsg, char *retrievedMsg) {
    int index = 0, oneCount = 0, msgLength;
    msgLength = strlen(stuffedMsg);
    for (index = 0; index < msgLength; index++) {
        if (oneCount == 5 && stuffedMsg[index] == '0') {
            oneCount = 0;
            continue;
        }
        if (stuffedMsg[index] == '1') {
            oneCount++;
        } else {
            oneCount = 0;
        }
        strncat(retrievedMsg, &stuffedMsg[index], 1);
    }
}

int main() {
    char inputMsg[1000], stuffedMsg[2000], retrievedMsg[1000], finalMsg[2000];
    
    while (1) {
        strcpy(stuffedMsg, "01111110");
        retrievedMsg[0] = '\0';
        finalMsg[0] = '\0';
        
        printf("Enter the message (or type 'exit' to stop): ");
        scanf("%s", inputMsg);

        
        if (strcmp(inputMsg, "exit") == 0) {
            break;
        }

        bitStuffing(inputMsg, stuffedMsg);
        strcat(stuffedMsg, "01111110");
        printf("Stuffed message: %s\n", stuffedMsg);
        
        reverseBitStuffing(stuffedMsg + 8, retrievedMsg);
        strcpy(finalMsg, "01111110");
        strcat(finalMsg, retrievedMsg);
        printf("Retrieved message: %s\n", finalMsg);
    }

    return 0;
}
