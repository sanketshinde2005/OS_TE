#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
sem_t db_access;
pthread_mutex_t mutex; 
int reader_count = 0;
int shared_data = 0;
void* reader(void* arg) {
    int id = *(int*)arg;
    while (1) {
        pthread_mutex_lock(&mutex);
        reader_count++;
        if (reader_count == 1)
            sem_wait(&db_access);
        pthread_mutex_unlock(&mutex);
        printf("Reader %d: read shared_data = %d\n", id, shared_data);
        sleep(1);
        pthread_mutex_lock(&mutex);
        reader_count--;
        if (reader_count == 0)
            sem_post(&db_access);
        pthread_mutex_unlock(&mutex);
        sleep(rand() % 3);
    }
}
void* writer(void* arg) {
    int id = *(int*)arg;
    while (1) {
        sem_wait(&db_access);
        shared_data++;
        printf("Writer %d: updated shared_data to %d\n", id, shared_data);
        sem_post(&db_access);
        sleep(rand() % 5);
    }
}
int main() {
    int i;
    pthread_t rtid[5], wtid[2];
    int r_ids[5], w_ids[2];
    sem_init(&db_access, 0, 1);
    pthread_mutex_init(&mutex, NULL);
    for (i = 0; i < 5; i++) {
        r_ids[i] = i + 1;
        pthread_create(&rtid[i], NULL, reader, &r_ids[i]);
    }
    for (i = 0; i < 2; i++) {
        w_ids[i] = i + 1;
        pthread_create(&wtid[i], NULL, writer, &w_ids[i]);
    }
    for (i = 0; i < 5; i++)
        pthread_join(rtid[i], NULL);
    for (i = 0; i < 2; i++)
        pthread_join(wtid[i], NULL);
    sem_destroy(&db_access);
    pthread_mutex_destroy(&mutex);
    return 0;
}