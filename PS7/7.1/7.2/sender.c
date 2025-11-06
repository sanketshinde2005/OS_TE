#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#define SHM_SIZE 1024  // Shared memory size
struct shared_data {
    int flag;           // 0 = empty, 1 = full
    char message[SHM_SIZE];
};
int main() {
    key_t key;
    int shmid;
    struct shared_data *shm_ptr;
    key = ftok("shmfile", 65);
    if (key == -1) {
        perror("ftok");
        exit(1);
    }
    shmid = shmget(key, sizeof(struct shared_data), 0666 | IPC_CREAT);
    if (shmid == -1) {
        perror("shmget");
        exit(1);
    }
    shm_ptr = (struct shared_data *) shmat(shmid, NULL, 0);
    if (shm_ptr == (void *) -1) {
        perror("shmat");
        exit(1);
    }
    shm_ptr->flag = 0;  // Initially empty
    printf("=== SERVER PROCESS ===\n");
    printf("Enter message to send: ");
    fgets(shm_ptr->message, SHM_SIZE, stdin);
    shm_ptr->message[strcspn(shm_ptr->message, "\n")] = '\0';
    shm_ptr->flag = 1;  // Mark as data ready
    printf("Message written to shared memory.\nWaiting for client to read it...\n");
    while (shm_ptr->flag != 0) {
        sleep(1);
    }
    printf("Client has read the message.\n");
    shmdt((void *) shm_ptr);
    shmctl(shmid, IPC_RMID, NULL);
    printf("Shared memory removed. Server exiting.\n");
    return 0;
}
