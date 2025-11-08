#include<stdio.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<string.h>
#include<ctype.h>
#include<stdlib.h>
struct msg_buffer
{
    long msg_type;
    char msg_text[100];
}message;
int main()
{
    key_t key;
    int msgid;
    key=ftok("progfile",65);
    msgid=msgget(key,0666 | IPC_CREAT);
    if(msgid==-1)
    {
        perror("msgget failed");
        exit(1);
    }
    printf("Receiver Process Started...\n");
    while(1)
    {
        if(msgrcv(msgid,&message,sizeof(message.msg_text),0,0)==-1)
        {
            perror("msgrcv failed");
            exit(1);
        }
        if(strcmp(message.msg_text,"exit")==0)
        {
            break;
        }
        for(int i=0;message.msg_text[i]!='\0';i++)
        {
            message.msg_text[i]=toupper(message.msg_text[i]);
        }
        printf("\nReceived Message (Type %ld): %s\n", message.msg_type, message.msg_text);
    }
    msgctl(msgid,IPC_RMID,NULL);
     printf("\nMessage Queue destroyed. Receiver exiting...\n");
    return 0;
}