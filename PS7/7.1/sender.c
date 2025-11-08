#include<stdio.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<string.h>
#include<stdlib.h>
struct msg_buffer {
    long msg_type;
    char msg_text[100];
} message;
int main() {
    key_t key;
    int msgid;
    key = ftok("progfile", 65);
    msgid = msgget(key, 0666 | IPC_CREAT);
    if (msgid == -1) {
        perror("msgget failed");
        exit(1);
    }
    printf("Sender Process Started...\n");
    printf("Enter messages to send (type 'exit' to quit)\n");
    while (1) {
        printf("\nEnter Msg type: ");
        scanf("%ld", &message.msg_type);
        getchar();
        printf("Enter Message: ");
        fgets(message.msg_text, sizeof(message.msg_text), stdin);
        message.msg_text[strcspn(message.msg_text, "\n")] = '\0';
        if (msgsnd(msgid, &message, sizeof(message.msg_text), 0) == -1) {
            perror("msgsnd failed");
            exit(1);
        }
        printf("Message sent successfully!\n");
        if (strcmp(message.msg_text, "exit") == 0) {
            break;
        }
    }
    printf("\nSender exiting...\n");
    return 0;
}