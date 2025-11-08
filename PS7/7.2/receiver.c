#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <unistd.h>

#define SHM_SIZE 1024

int main()
{
    key_t key;
    int shmid;
    char* str;
    key=ftok("shmfile",65);

    shmid=shmget(key,SHM_SIZE,0666);

     if (shmid == -1) {
        perror("shmget failed");
     }

    str=(char*)shmat(shmid,NULL,0);
    if (str == (char*) -1) {
        perror("shmat failed");
        exit(1);
    }
    printf("[Client] Data read from shared memory: %s\n", str);

    shmdt(str);

    return 0;
}