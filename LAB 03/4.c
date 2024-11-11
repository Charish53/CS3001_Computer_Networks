#include <stdio.h>
#include <string.h>
void appendzeros(char *message, int m) {
int len = strlen(message);
for (int i = 0; i < m - 1; i++) {
    message[len + i] = '0';
    }
    message[len + m - 1] = '\0';
    }
void performDivision(char *message, char *divisor, char *remainder) {
int len_msg = strlen(message);
int len_div = strlen(divisor);
strcpy(remainder, message);
for (int i = 0; i <= len_msg - len_div; i++) {
if (remainder[i] == '1') {
for (int j = 0; j < len_div; j++) {
remainder[i + j] = remainder[i + j] == divisor[j] ? '0' : '1';
}
}
}
}
void generateFinalMessage(char *message, char *remainder, int m) {
int len_msg = strlen(message);
int len_rem = strlen(remainder);
message[len_msg - (m - 1)] = '\0';
strcat(message, remainder + len_rem - (m - 1));
}
int checkError(char *remainder2){
int len_rem = strlen(remainder2);
for (int i=0;i<len_rem;i++){
if (remainder2[i]=='1')
return 0;
}
return 1;
}
int main() {
char message[100];
char divisor[100];
char remainder[200]; 
char remainder2[200]; 
printf("Enter the binary message: ");
scanf("%s", message);
printf("Enter the binary divisor: ");
scanf("%s", divisor);
int m = strlen(divisor); 
appendzeros(message, m);
performDivision(message, divisor, remainder);
generateFinalMessage(message, remainder, m);
printf("Final message to be sent with CRC: %s\n", message);
performDivision(message, divisor, remainder2);
if (checkError(remainder2))
printf("message recieved correctly");
else
printf("error in recieved message!!");
return 0;
}