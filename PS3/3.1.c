#include <stdio.h>
struct process {
    char name[5];
    int arrival, burst, waiting, turnaround, completed;
};
void sortByArrival(struct process p[], int n) {
    struct process temp;
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (p[i].arrival > p[j].arrival) {
                temp = p[i];
                p[i] = p[j];
                p[j] = temp;
            }
        }
    }
}
void FCFS(struct process p[], int n) {
    int time = 0;
    float totalWT = 0, totalTAT = 0;
    sortByArrival(p, n);
    printf("\nGANTT CHART:\n");
    printf("_____________________________________\n|");
    for (int i = 0; i < n; i++) {
        if (time < p[i].arrival)
            time = p[i].arrival;  // CPU idle till process arrives
        time += p[i].burst;
        p[i].turnaround = time - p[i].arrival;
        p[i].waiting = p[i].turnaround - p[i].burst;

        totalWT += p[i].waiting;
        totalTAT += p[i].turnaround;

        printf(" %s |", p[i].name);
    }
    printf("\n_____________________________________\n0");
    time = 0;
    for (int i = 0; i < n; i++) {
        if (time < p[i].arrival)
            time = p[i].arrival;
        time += p[i].burst;
        printf("\t%d", time);
    }
    printf("\n\nAverage Waiting Time: %.2f", totalWT / n);
    printf("\nAverage Turnaround Time: %.2f\n", totalTAT / n);
}
void SJF(struct process p[], int n) {
    int completed = 0, time = 0;
    float totalWT = 0, totalTAT = 0;
    struct process temp;

    for (int i = 0; i < n; i++) p[i].completed = 0;

    printf("\nGANTT CHART:\n");
    printf("_____________________________________\n|");

    while (completed != n) {
        int idx = -1, minBT = 9999;

        for (int i = 0; i < n; i++) {
            if (p[i].arrival <= time && !p[i].completed && p[i].burst < minBT) {
                minBT = p[i].burst;
                idx = i;
            }
        }
        if (idx == -1) { // No process has arrived yet
            time++;
            continue;
        }
        time += p[idx].burst;
        p[idx].turnaround = time - p[idx].arrival;
        p[idx].waiting = p[idx].turnaround - p[idx].burst;
        p[idx].completed = 1;
        completed++;
        totalWT += p[idx].waiting;
        totalTAT += p[idx].turnaround;
        printf(" %s |", p[idx].name);
    }
    printf("\n_____________________________________\n0");
    time = 0;
    completed = 0;
    while (completed != n) {
        int idx = -1, minBT = 9999;
        for (int i = 0; i < n; i++) {
            if (p[i].arrival <= time && p[i].completed == 2) continue;
            if (p[i].arrival <= time && p[i].completed == 1 && p[i].burst < minBT) {
                minBT = p[i].burst;
                idx = i;
            }
        }
        if (idx == -1) {
            time++;
            continue;
        }
        time += p[idx].burst;
        p[idx].completed = 2;
        printf("\t%d", time);
        completed++;
    }
    printf("\n\nAverage Waiting Time: %.2f", totalWT / n);
    printf("\nAverage Turnaround Time: %.2f\n", totalTAT / n);
}
int main() {
    int n, choice;
    struct process p[10];
    printf("Enter number of processes: ");
    scanf("%d", &n);
    for (int i = 0; i < n; i++) {
        printf("Process name: ");
        scanf("%s", p[i].name);
        printf("Arrival time: ");
        scanf("%d", &p[i].arrival);
        printf("Burst time: ");
        scanf("%d", &p[i].burst);
    }
    do {
        printf("\n\nMenu:\n1. FCFS\n2. SJF (Non-Preemptive)\n3. Exit\nEnter your choice: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                FCFS(p, n);
                break;
            case 2:
                SJF(p, n);
                break;
            case 3:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice.\n");
        }
    } while (choice != 3);
    return 0;
}
