#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#define MAX 100
struct msg_buffer {
    long msg_type;
    char msg_text[MAX];
};
void toUpperCase(char *str) {
    for (int i = 0; str[i] != '\0'; i++)
        str[i] = toupper(str[i]);
}
int main() {
    key_t key;
    int msgid;
    struct msg_buffer message;
    key = ftok("msgfile", 65);
    if (key == -1) {
        perror("ftok");
        exit(1);
    }
    msgid = msgget(key, 0666 | IPC_CREAT);
    if (msgid == -1) {
        perror("msgget");
        exit(1);
    }
    printf("=== RECEIVER PROCESS ===\n\n");
    while (1) {
        if (msgrcv(msgid, &message, sizeof(message.msg_text), 0, 0) == -1) {
            perror("msgrcv");
            exit(1);
        }
        if (strcmp(message.msg_text, "exit") == 0) {
            printf("Received 'exit' message. Cleaning up...\n");
            msgctl(msgid, IPC_RMID, NULL); // Remove queue
            printf("Message queue deleted.\n");
            break;
        }
        toUpperCase(message.msg_text);
        printf("[Type: %ld] Message received: %s\n", message.msg_type, message.msg_text);
    }
    return 0;
}