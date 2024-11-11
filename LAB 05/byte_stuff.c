//CS22B1095 REDDIPALLI SAI CHARISH
#include <stdio.h>
#include <string.h>

#define FLAG 0x7E  // Flag byte
#define ESCAPE 0x7D // Escape byte

void byte_stuffing(unsigned char data[], int len, unsigned char stuffed_data[], int *stuffed_len) {
    int j = 0;


    stuffed_data[j++] = FLAG;

    for (int i = 0; i < len; i++) {
        if (data[i] == FLAG) {
            stuffed_data[j++] = ESCAPE;
            stuffed_data[j++] = 0x5E; // Replace 0x7E with 0x7D 0x5E
        } else if (data[i] == ESCAPE) {
            stuffed_data[j++] = ESCAPE;
            stuffed_data[j++] = 0x5D; // Replace 0x7D with 0x7D 0x5D
        } else {
            stuffed_data[j++] = data[i];
        }
    }

    // Add FLAG byte at the end of the stuffed data
    stuffed_data[j++] = FLAG;

    *stuffed_len = j;
}

// Function to perform byte de-stuffing
void byte_destuffing(unsigned char stuffed_data[], int stuffed_len, unsigned char destuffed_data[], int *destuffed_len) {
    int j = 0;

    // Ignore the FLAG byte at the beginning
    for (int i = 1; i < stuffed_len - 1; i++) { // Ignore first and last FLAG byte
        if (stuffed_data[i] == ESCAPE) {
            if (stuffed_data[i + 1] == 0x5E) {
                destuffed_data[j++] = FLAG; // Convert 0x7D 0x5E back to 0x7E
            } else if (stuffed_data[i + 1] == 0x5D) {
                destuffed_data[j++] = ESCAPE; // Convert 0x7D 0x5D back to 0x7D
            }
            i++; // Skip the next byte as it is part of the escape sequence
        } else {
            destuffed_data[j++] = stuffed_data[i];
        }
    }
    *destuffed_len = j;
}

int main() {
    unsigned char data[100];  // To store user input data
    int len;

    printf("Enter the number of data bytes: ");
    scanf("%d", &len);

    printf("Enter the data bytes in hexadecimal (e.g., 45 7E 56): \n");
    for (int i = 0; i < len; i++) {
        unsigned int input;
        scanf("%x", &input);
        data[i] = (unsigned char)input;
    }

    unsigned char stuffed_data[200];  // To store the stuffed data
    int stuffed_len;

    unsigned char destuffed_data[100];  // To store the destuffed data
    int destuffed_len;

    printf("Original data: ");
    for (int i = 0; i < len; i++) {
        printf("0x%X ", data[i]);
    }
    printf("\n");

    // Perform byte stuffing
    byte_stuffing(data, len, stuffed_data, &stuffed_len);

    printf("Stuffed data with flag: ");
    for (int i = 0; i < stuffed_len; i++) {
        printf("0x%X ", stuffed_data[i]);
    }
    printf("\n");

    // Perform byte de-stuffing
    byte_destuffing(stuffed_data, stuffed_len, destuffed_data, &destuffed_len);

    printf("De-stuffed data: ");
    for (int i = 0; i < destuffed_len; i++) {
        printf("0x%X ", destuffed_data[i]);
    }
    printf("\n");

    return 0;
}
