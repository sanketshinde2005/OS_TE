#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
void sort(int arr[], int n) {
    for(int i=0; i<n-1; i++) {
        for(int j=0; j<n-i-1; j++) {
            if(arr[j] > arr[j+1]) {
                int temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
            }
        }
    }
}
int main() {
    int n;
    printf("Enter number of elements: ");
    scanf("%d", &n);
    int arr[n];
    printf("Enter %d elements: ", n);
    for(int i=0; i<n; i++)
        scanf("%d", &arr[i]);
    pid_t pid = fork();
    if (pid < 0) {
        perror("Fork failed");
        return 1;
    }
    else if (pid == 0) {
    }
    else {
        sort(arr, n);
        printf("\nSorted Array: ");
        for(int i=0; i<n; i++)
            printf("%d ", arr[i]);
        printf("\n");
        char *args[n + 3];
        args[0] = "./child";
        for(int i=0; i<n; i++) {
            char *num = malloc(10);
            sprintf(num, "%d", arr[i]);
            args[i+1] = num;
        }
        args[n+1] = NULL;
        char *envp[] = {NULL};
        execve("./child", args, envp);
        perror("execve failed");
        wait(NULL);
    }
    return 0;
}