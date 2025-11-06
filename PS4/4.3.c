#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>
#define FILENAME "shared_buffer.txt"
pthread_mutex_t mutex;   // Mutual exclusion for file access
sem_t can_produce;       // Binary semaphore – signals producer can write
sem_t can_consume;       // Binary semaphore – signals consumer can read
void *producer(void *arg) {
    FILE *fp;
    int num;
    while (1) {
        sem_wait(&can_produce);          // Wait until allowed to produce
        pthread_mutex_lock(&mutex);      // Lock file for writing
        num = rand() % 10;               // Random integer (0–9)
        fp = fopen(FILENAME, "w");       // Overwrite the file
        if (fp == NULL) {
            perror("Error opening file for writing");
            exit(1);
        }
        fprintf(fp, "%d", num);
        fclose(fp);
        printf("👨‍🍳 Producer wrote number: %d\n", num);
        pthread_mutex_unlock(&mutex);    // Unlock file
        sem_post(&can_consume);          // Signal consumer
        usleep((rand() % 101) * 1000)
    }
}
void *consumer(void *arg) {
    FILE *fp;
    int num;
    while (1) {
        sem_wait(&can_consume);          // Wait until data is ready
        pthread_mutex_lock(&mutex);      // Lock file for reading
        fp = fopen(FILENAME, "r");
        if (fp == NULL) {
            perror("Error opening file for reading");
            exit(1);
        }
        fscanf(fp, "%d", &num);
        fclose(fp);
        printf("😋 Consumer read number: %d\n", num);
        pthread_mutex_unlock(&mutex);    // Unlock file
        sem_post(&can_produce);          // Signal producer to produce again
        usleep(50000);                   // Sleep 50 ms (simulate processing)
    }
}
int main() {
    srand(time(NULL));
    pthread_t prod, cons;
    pthread_mutex_init(&mutex, NULL);
    sem_init(&can_produce, 0, 1); // Producer starts first
    sem_init(&can_consume, 0, 0); // Consumer waits initially
    pthread_create(&prod, NULL, producer, NULL);
    pthread_create(&cons, NULL, consumer, NULL);
    pthread_join(prod, NULL);
    pthread_join(cons, NULL);
    pthread_mutex_destroy(&mutex);
    sem_destroy(&can_produce);
    sem_destroy(&can_consume);
    return 0;
}
