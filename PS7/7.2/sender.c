#include<stdio.h>
#include<stdlib.h>
#include<sys/shm.h>
#include<sys/ipc.h>
#include<unistd.h>
#include<string.h>

#define SHM_SIZE 1024

int main()
{
    key_t key;
    int shmid;
    char* str;
    key=ftok("shmfile",65);
    shmid=shmget(key,SHM_SIZE,0666 | IPC_CREAT);
    if(shmid==-1)
    {
        perror("shmget failed");
        exit(1);
    }
    str=(char*)shmat(shmid,NULL,0);
    if(str==(char*)-1)
    {
        perror("shmat failed");
        exit(1);
    }
    printf("enter a message");
    fgets(str,SHM_SIZE,stdin);
    printf("\n[Server] Data written to shared memory: %s\n", str);
    printf("[Server] Waiting for client to read the data...\n");

    sleep(10);
    shmdt(str);
    shmctl(shmid,IPC_RMID,NULL);

    printf("[Server] Shared memory detached and removed.\n");
    return 0;
}