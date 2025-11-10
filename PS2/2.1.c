#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
void bubbleSort(int arr[], int n) {
    int i, j, temp;
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}
void insertionSort(int arr[], int n) {
    int i, key, j;
    for (i = 1; i < n; i++) {
        key = arr[i];
        j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}
void displayArray(int arr[], int n) {
    for (int i = 0; i < n; i++)
        printf("%d ", arr[i]);
    printf("\n");
}
int main() {
    int n;
    printf("Enter number of elements: ");
    scanf("%d", &n);
    int arr[n];
    printf("Enter %d integers:\n", n);
    for (int i = 0; i < n; i++)
        scanf("%d", &arr[i]);
    pid_t pid1 = fork();
    if (pid1 == 0) {
        printf("\n[Child1 - ORPHAN] PID: %d | PPID: %d\n", getpid(), getppid());
        insertionSort(arr, n);
        printf("[Child1] After sorting: ");
        displayArray(arr, n);
        printf("[Child1] Sleeping to become ORPHAN...\n");
        sleep(5);
        printf("[Child1] After sleep → new PPID = %d (ORPHAN OK)\n", getppid());
        exit(0);
    }
    pid_t pid2 = fork();
    if (pid2 == 0) {
        printf("\n[Child2 - ZOMBIE] PID: %d | PPID: %d\n", getpid(), getppid());
        printf("[Child2] Exiting immediately → will become ZOMBIE.\n");
        exit(0);
    }
    printf("\n[Parent] PID: %d | Child1: %d | Child2: %d\n", getpid(), pid1, pid2);
    bubbleSort(arr, n);
    printf("[Parent] Bubble Sort done: ");
    displayArray(arr, n);
    printf("\n[Parent] Sleeping 10 seconds so ZOMBIE is visible...\n");
    sleep(10);
    printf("\n[Parent] Reaping only Child2 (ZOMBIE)...\n");
    waitpid(pid2, NULL, 0);
    printf("[Parent] Now waiting for Child1 (ORPHAN reaped by systemd).\n");
    waitpid(pid1, NULL, 0);
    printf("[Parent] Done.\n");
    return 0;
}
