#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define BUFFER_SIZE 5   // Max pizzas on counter
int counter = 0;         // Current number of pizzas
sem_t empty;             // Counts empty slots on counter
sem_t full;              // Counts filled slots
pthread_mutex_t mutex;   // Mutual exclusion for counter access
void *producer(void *arg) {
    while (1) {
        sem_wait(&empty);           // Wait if counter is full
        pthread_mutex_lock(&mutex); // Lock counter access

        counter++;
        printf("👨‍🍳 Mr. Simpson baked a pizza 🍕 | Total pizzas on counter: %d\n", counter);

        pthread_mutex_unlock(&mutex); // Unlock counter
        sem_post(&full);              // Signal that pizza is available

        sleep(1); // Simulate baking time
    }
}
void *consumer(void *arg) {
    while (1) {
        sem_wait(&full);             // Wait if counter is empty
        pthread_mutex_lock(&mutex);  // Lock counter access

        counter--;
        printf("😋 Joey ate a pizza 🍴 | Pizzas left on counter: %d\n", counter);

        pthread_mutex_unlock(&mutex); // Unlock counter
        sem_post(&empty);             // Signal empty space available

        sleep(2); // Simulate eating time
    }
}
int main() {
    pthread_t tid1, tid2;
    sem_init(&empty, 0, BUFFER_SIZE);  // 'empty' = BUFFER_SIZE initially
    sem_init(&full, 0, 0);             // 'full' = 0 initially
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
