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
    pid_t pid = fork();
    if (pid < 0) {
        perror("Fork failed");
        exit(1);
    }
    else if (pid == 0) {
        printf("\n[Child] PID: %d | PPID: %d\n", getpid(), getppid());
        printf("[Child] Performing Insertion Sort...\n");
        insertionSort(arr, n);
        printf("[Child] Sorted Array (Insertion Sort): ");
        displayArray(arr, n);
        printf("[Child] Sleeping 5 seconds to become ORPHAN...\n");
        sleep(5);
        printf("[Child] After sleep, new PPID = %d (ORPHAN confirmed)\n", getppid());
        printf("[Child] Exiting now → will briefly become ZOMBIE.\n");
        exit(0);
    }
    else {
        printf("\n[Parent] PID: %d | Child PID: %d\n", getpid(), pid);
        printf("[Parent] Performing Bubble Sort...\n");
        bubbleSort(arr, n);
        printf("[Parent] Sorted Array (Bubble Sort): ");
        displayArray(arr, n);
        printf("[Parent] Exiting immediately → child becomes ORPHAN.\n");
        exit(0);
    }
    return 0;
}