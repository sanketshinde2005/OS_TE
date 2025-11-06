#include <stdio.h>
#include <stdlib.h>
#define MAX 10
#define CYLINDERS 500
void sort(int arr[], int n) {
    int temp;
    for (int i = 0; i < n - 1; i++)
        for (int j = i + 1; j < n; j++)
            if (arr[i] > arr[j]) {
                temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
}
void SCAN(int requests[], int n, int head) {
    int total = 0;
    int seq[MAX + 2], index = 0;
    int temp[MAX];
    for (int i = 0; i < n; i++) temp[i] = requests[i];
    sort(temp, n);
    int pos = 0;
    while (pos < n && temp[pos] < head) pos++;
    printf("\n===== SCAN Scheduling =====\n");
    printf("Seek Sequence: %d", head);
    for (int i = pos; i < n; i++) {
        total += abs(temp[i] - head);
        head = temp[i];
        seq[index++] = head;
    }
    total += abs((CYLINDERS - 1) - head);
    head = CYLINDERS - 1;
    for (int i = pos - 1; i >= 0; i--) {
        total += abs(temp[i] - head);
        head = temp[i];
        seq[index++] = head;
    }
    printf(" ->");
    for (int i = 0; i < index; i++) printf(" %d", seq[i]);
    printf("\nTotal Head Movement = %d", total);
    printf("\nAverage Seek Distance = %.2f\n", (float) total / n);
}
void LOOK(int requests[], int n, int head) {
    int total = 0;
    int seq[MAX + 1], index = 0;
    int temp[MAX];
    for (int i = 0; i < n; i++) temp[i] = requests[i];
    sort(temp, n);
    int pos = 0;
    while (pos < n && temp[pos] < head) pos++;
    printf("\n===== LOOK Scheduling =====\n");
    printf("Seek Sequence: %d", head);
    for (int i = pos; i < n; i++) {
        total += abs(temp[i] - head);
        head = temp[i];
        seq[index++] = head;
    }
    for (int i = pos - 1; i >= 0; i--) {
        total += abs(temp[i] - head);
        head = temp[i];
        seq[index++] = head;
    }
    printf(" ->");
    for (int i = 0; i < index; i++) printf(" %d", seq[i]);
    printf("\nTotal Head Movement = %d", total);
    printf("\nAverage Seek Distance = %.2f\n", (float) total / n);
}
int main() {
    int requests[MAX] = {20, 229, 39, 450, 18, 145, 120, 380, 20, 250};
    int head = 185;
    int n = 10;
    printf("SPPU OS LAB Q8.2 - Disk Scheduling Algorithms\n");
    printf("Requests: ");
    for (int i = 0; i < n; i++) printf("%d ", requests[i]);
    printf("\nInitial Head Position: %d\n", head);
    SCAN(requests, n, head);
    LOOK(requests, n, head);
    return 0;
}