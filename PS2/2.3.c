#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
int isPrime(int num) {
    if (num < 2) return 0;
    for (int i = 2; i * i <= num; i++) {
        if (num % i == 0)
            return 0;
    }
    return 1;
}
int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <positive_integer>\n", argv[0]);
        exit(1);
    }
    int n = atoi(argv[1]);
    if (n <= 0) {
        fprintf(stderr, "Error: Please provide a positive integer.\n");
        exit(1);
    }
    pid_t pid = fork();
    if (pid < 0) {
        perror("Fork failed");
        exit(1);
    }
    else if (pid == 0) {
        printf("Child process (PID: %d) generating first %d prime numbers:\n", getpid(), n);
        int count = 0, num = 2;
        while (count < n) {
            if (isPrime(num)) {
                printf("%d ", num);
                count++;
            }
            num++;
        }
        printf("\nChild process completed.\n");
        exit(0);
    }
    else {
        printf("Parent process (PID: %d) waiting for child...\n", getpid());
        wait(NULL);
        printf("Parent process: Child completed successfully.\n");
    }
    return 0;
}