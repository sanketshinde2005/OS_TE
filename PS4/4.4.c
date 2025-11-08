#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
sem_t db_access;          // semaphore for database access
pthread_mutex_t mutex;    // mutex for reader count
int reader_count = 0;     // number of readers currently accessing
int shared_data = 0;      // the flight data (shared resource)
void* reader(void* arg) {
    int id = *(int*)arg;
    while (1) {
        pthread_mutex_lock(&mutex);
        reader_count++;
        if (reader_count == 1)
            sem_wait(&db_access);  // first reader blocks writers
        pthread_mutex_unlock(&mutex);
        printf("Reader %d: read shared_data = %d\n", id, shared_data);
        sleep(1);
        pthread_mutex_lock(&mutex);
        reader_count--;
        if (reader_count == 0)
            sem_post(&db_access);  // last reader allows writers
        pthread_mutex_unlock(&mutex);
        sleep(rand() % 3);  // simulate random arrival time
    }
}
void* writer(void* arg) {
    int id = *(int*)arg;
    while (1) {
        sem_wait(&db_access);   // only one writer (or readers group) can enter
        shared_data++;          // modify shared data
        printf("Writer %d: updated shared_data to %d\n", id, shared_data);
        sem_post(&db_access);   // release database
        sleep(rand() % 5);      // simulate delay
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