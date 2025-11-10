#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#define BUFFER_SIZE 5
int total = 0;
int counter = 0; 
sem_t empty; 
sem_t full;
pthread_mutex_t mutex;
void *producer(void *arg) {
    while (1) {
        if (total == 20) {
            pthread_exit(NULL);
        }
        sem_wait(&empty);
        pthread_mutex_lock(&mutex);
        counter++;
        total++;
        printf("Mr. Simpson baked a pizza| Total pizzas on counter: %d | Produced : %d\n", counter, total);
        pthread_mutex_unlock(&mutex);
        sem_post(&full);
        sleep(1);
    }
}
void *consumer(void *arg) {
    while (1) {
        if (total == 20 && counter == 0) {
            pthread_exit(NULL);
        }
        sem_wait(&full); 
        pthread_mutex_lock(&mutex);
        counter--;
        printf("Joey ate a pizza | Pizzas left on counter: %d\n", counter);

        pthread_mutex_unlock(&mutex);
        sem_post(&empty);
        sleep(2);
    }
}
int main() {
    pthread_t tid1, tid2;
    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);
    pthread_mutex_init(&mutex, NULL);
    pthread_create(&tid1, NULL, producer, NULL);
    pthread_create(&tid2, NULL, consumer, NULL);
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    pthread_mutex_destroy(&mutex);
    sem_destroy(&empty);
    sem_destroy(&full);

    return 0;
}
