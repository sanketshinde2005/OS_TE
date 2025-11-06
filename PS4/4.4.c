#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
sem_t wrt;               // Semaphore for writer access to the shared data
pthread_mutex_t mutex;   // Protects readcount variable
int readcount = 0;       // Number of readers currently reading
int data = 1;            // Shared data (flight info)
void *reader(void *arg) {
    int id = *(int *)arg;
    while (1) {
        pthread_mutex_lock(&mutex);
        readcount++;
        if (readcount == 1)
            sem_wait(&wrt); // First reader locks writers ou
        pthread_mutex_unlock(&mutex);
        printf("🧾 Reader %d is reading flight info: [Available Seats = %d]\n", id, data);
        sleep(rand() % 3 + 1); // Simulate reading time

        pthread_mutex_lock(&mutex);
        readcount--;
        if (readcount == 0)
            sem_post(&wrt); // Last reader allows writers
        pthread_mutex_unlock(&mutex);
        sleep(rand() % 5 + 1); // Wait before next read attempt
    }
}
void *writer(void *arg) {
    int id = *(int *)arg;
    while (1) {
        sem_wait(&wrt); // Wait until no readers/writers accessing shared data
        printf("🧑‍💻 Writer %d is booking a seat...\n", id);
        data--; // Simulate booking (reduce available seats)
        printf("✅ Writer %d updated flight info: [Available Seats = %d]\n", id, data);
        sleep(rand() % 3 + 1); // Simulate writing time
        sem_post(&wrt); // Release access to database
        sleep(rand() % 6 + 1); // Wait before next booking attempt
    }
}
int main() {
    srand(time(NULL));
    pthread_t rtid[5], wtid[2];
    int r_id[5], w_id[2];
    sem_init(&wrt, 0, 1);
    pthread_mutex_init(&mutex, NULL);
    for (int i = 0; i < 5; i++) {
        r_id[i] = i + 1;
        pthread_create(&rtid[i], NULL, reader, &r_id[i]);
    }
    for (int i = 0; i < 2; i++) {
        w_id[i] = i + 1;
        pthread_create(&wtid[i], NULL, writer, &w_id[i]);
    }
    for (int i = 0; i < 5; i++)
        pthread_join(rtid[i], NULL);
    for (int i = 0; i < 2; i++)
        pthread_join(wtid[i], NULL);
    pthread_mutex_destroy(&mutex);
    sem_destroy(&wrt);
    return 0;
}