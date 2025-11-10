#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>
#define FILE_NAME "shared.txt"
pthread_mutex_t mutex;
sem_t empty;
sem_t full;
void* producer(void* arg);
void* consumer(void* arg);
void* producer(void* arg) {
    while (1) {
        sem_wait(&empty);
        pthread_mutex_lock(&mutex);
        int num = rand() % 10;
        FILE* fp = fopen(FILE_NAME, "w");
        if (fp == NULL) {
            perror("Error opening file for writing");
            exit(1);
        }
        fprintf(fp, "%d\n", num);
        fclose(fp);
        printf("Producer: produced %d\n", num);
        pthread_mutex_unlock(&mutex);
        sem_post(&full);
        usleep((rand() % 101) * 1000);
    }
    return NULL;
}
void* consumer(void* arg) {
    while (1) {
        sem_wait(&full);
        pthread_mutex_lock(&mutex);
        FILE* fp = fopen(FILE_NAME, "r");
        if (fp == NULL) {
            perror("Error opening file for reading");
            exit(1);
        }
        int num;
        fscanf(fp, "%d", &num);
        fclose(fp);
        printf("Consumer: consumed %d\n", num);
        pthread_mutex_unlock(&mutex);
        sem_post(&empty);
    }
    return NULL;
}
int main() {
    srand(time(NULL));
    pthread_t prodThread, consThread;
    pthread_mutex_init(&mutex, NULL);
    sem_init(&empty, 0, 1); // File initially empty
    sem_init(&full, 0, 0);
    pthread_create(&prodThread, NULL, producer, NULL);
    pthread_create(&consThread, NULL, consumer, NULL);
    pthread_join(prodThread, NULL);
    pthread_join(consThread, NULL);
    pthread_mutex_destroy(&mutex);
    sem_destroy(&empty);
    sem_destroy(&full);
    return 0;
}