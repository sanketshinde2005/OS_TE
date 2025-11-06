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
void CSCAN(int requests[], int n, int head) {
    int total = 0;
    int seq[2 * MAX], index = 0;
    int temp[MAX];
    for (int i = 0; i < n; i++) temp[i] = requests[i];
    sort(temp, n);
    int pos = 0;
    while (pos < n && temp[pos] < head) pos++;
    printf("\n===== C-SCAN Scheduling =====\n");
    printf("Seek Sequence: %d", head);
    for (int i = pos; i < n; i++) {
        total += abs(temp[i] - head);
        head = temp[i];
        seq[index++] = head;
    }
    if (head != CYLINDERS - 1) {
        total += abs((CYLINDERS - 1) - head);
        head = 0;
        total += CYLINDERS - 1; // jump from 499 to 0
    }
    for (int i = 0; i < pos; i++) {
        total += abs(temp[i] - head);
        head = temp[i];
        seq[index++] = head;
    }
    printf(" ->");
    for (int i = 0; i < index; i++) printf(" %d", seq[i]);
    printf("\nTotal Head Movement = %d\n", total);
    printf("Average Seek Distance = %.2f\n", (float) total / n);
}
void CLOOK(int requests[], int n, int head) {
    int total = 0;
    int seq[MAX], index = 0;
    int temp[MAX];
    for (int i = 0; i < n; i++) temp[i] = requests[i];
    sort(temp, n);
    int pos = 0;
    while (pos < n && temp[pos] < head) pos++;
    printf("\n===== C-LOOK Scheduling =====\n");
    printf("Seek Sequence: %d", head);
    for (int i = pos; i < n; i++) {
        total += abs(temp[i] - head);
        head = temp[i];
        seq[index++] = head;
    }
    if (pos > 0) {
        total += abs(head - temp[0]);
        head = temp[0];
        seq[index++] = head;
    }
    for (int i = 1; i < pos; i++) {
        total += abs(temp[i] - head);
        head = temp[i];
        seq[index++] = head;
    }
    printf(" ->");
    for (int i = 0; i < index; i++) printf(" %d", seq[i]);
    printf("\nTotal Head Movement = %d\n", total);
    printf("Average Seek Distance = %.2f\n", (float) total / n);
}
int main() {
    int requests[MAX] = {10, 229, 39, 400, 18, 145, 120, 480, 20, 250};
    int head = 85;
    int n = 10;
    printf("SPPU OS LAB Q8.1 - Disk Scheduling Algorithms\n");
    printf("Requests: ");
    for (int i = 0; i < n; i++) printf("%d ", requests[i]);
    printf("\nInitial Head Position: %d\n", head);
    CSCAN(requests, n, head);
    CLOOK(requests, n, head);
    return 0;
}