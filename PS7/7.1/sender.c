#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#define MAX 100
struct msg_buffer {
    long msg_type;
    char msg_text[MAX];
};
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
    printf("=== SENDER PROCESS ===\n");
    printf("Type 'exit' to quit.\n\n");
    while (1) {
        printf("Enter message type (integer): ");
        scanf("%ld", &message.msg_type);
        getchar(); // consume newline
        printf("Enter message text: ");
        fgets(message.msg_text, MAX, stdin);
        message.msg_text[strcspn(message.msg_text, "\n")] = '\0'; // remove newline
        if (msgsnd(msgid, &message, sizeof(message.msg_text), 0) == -1) {
            perror("msgsnd");
            exit(1);
        }
        if (strcmp(message.msg_text, "exit") == 0) {
            printf("Exiting Sender...\n");
            break;
        }
        printf("Message sent successfully!\n\n");
    }
    return 0;
}