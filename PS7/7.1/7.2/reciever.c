#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <string.h>
#define SHM_SIZE 1024
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
    shmid = shmget(key, sizeof(struct shared_data), 0666);
    if (shmid == -1) {
        perror("shmget");
        exit(1);
    }
    shm_ptr = (struct shared_data *) shmat(shmid, NULL, 0);
    if (shm_ptr == (void *) -1) {
        perror("shmat");
        exit(1);
    }
    printf("=== CLIENT PROCESS ===\n");
    printf("Waiting for message from server...\n");
    while (shm_ptr->flag == 0) {
        sleep(1);
    }
    printf("Message received from server: %s\n", shm_ptr->message);
    shm_ptr->flag = 0;
    shmdt((void *) shm_ptr);
    printf("Client done. Exiting.\n");
    return 0;
}