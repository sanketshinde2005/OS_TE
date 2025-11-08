#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#define MAX_CHAIRS 3   // chairs in the hallway
#define MAX_STUDENTS 8 // total students to simulate
sem_t students;        // counts waiting students
sem_t ta;              // signals TA availability
pthread_mutex_t mutex; // mutual exclusion for chairs
int waiting = 0;       // number of students currently waiting
void *TA(void *arg) {
    while (1) {
        sem_wait(&students);          // wait for a student to arrive
        pthread_mutex_lock(&mutex);   // access waiting count
        waiting--; // one student goes in for help
        printf("TA starts helping a student. Students waiting: %d\n", waiting);
        pthread_mutex_unlock(&mutex);
        sem_post(&ta); // signal TA is ready to help
        sleep(2);
        printf("TA finished helping the student and is now available.\n");
    }
}
void *student(void *arg) {
    int id = *(int *)arg;
    while (1) {
        sleep(rand() % 5 + 1); // students arrive randomly
        pthread_mutex_lock(&mutex);
        if (waiting < MAX_CHAIRS) {
            waiting++;
            printf("Student %d is waiting. Waiting students: %d\n", id, waiting);
            pthread_mutex_unlock(&mutex);
            sem_post(&students); // notify TA that a student arrived
            sem_wait(&ta);       // wait until TA is ready
            printf("Student %d is getting help from TA.\n", id);
            sleep(2); // being helped
            printf("Student %d got help and leaves.\n", id);
        } else {
            pthread_mutex_unlock(&mutex);
            printf("Student %d found no empty chair and will come back later.\n", id);
        }
    }
}
int main() {
    srand(time(NULL));
    pthread_t ta_thread;
    pthread_t student_threads[MAX_STUDENTS];
    int student_ids[MAX_STUDENTS];
    sem_init(&students, 0, 0);
    sem_init(&ta, 0, 0);
    pthread_mutex_init(&mutex, NULL);
    pthread_create(&ta_thread, NULL, TA, NULL);
    for (int i = 0; i < MAX_STUDENTS; i++) {
        student_ids[i] = i + 1;
        pthread_create(&student_threads[i], NULL, student, &student_ids[i]);
    }
    pthread_join(ta_thread, NULL);
    for (int i = 0; i < MAX_STUDENTS; i++)
        pthread_join(student_threads[i], NULL);
    pthread_mutex_destroy(&mutex);
    sem_destroy(&students);
    sem_destroy(&ta);
    return 0;
}
