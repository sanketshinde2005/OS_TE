#include <stdio.h>

struct process {
    char name[5];
    int arrival, burst, remaining, waiting, turnaround;
};

// ---------- FCFS Scheduling ----------
void FCFS(struct process p[], int n) {
    int time = 0;
    float totalWT = 0, totalTAT = 0;

    // Sort by arrival time
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (p[i].arrival > p[j].arrival) {
                struct process temp = p[i];
                p[i] = p[j];
                p[j] = temp;
            }
        }
    }

    printf("\nGANTT CHART:\n");
    printf("_____________________________________\n|");

    for (int i = 0; i < n; i++) {
        if (time < p[i].arrival)
            time = p[i].arrival;  // CPU idle
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

// ---------- Round Robin Scheduling ----------
void RoundRobin(struct process p[], int n, int quantum) {
    int completed = 0, time = 0, i = 0;
    float totalWT = 0, totalTAT = 0;
    int done;

    for (i = 0; i < n; i++) {
        p[i].remaining = p[i].burst;
        p[i].waiting = 0;
        p[i].turnaround = 0;
    }

    printf("\nGANTT CHART:\n");
    printf("_____________________________________\n|");

    while (1) {
        done = 1;

        for (i = 0; i < n; i++) {
            if (p[i].remaining > 0 && p[i].arrival <= time) {
                done = 0;

                if (p[i].remaining > quantum) {
                    time += quantum;
                    p[i].remaining -= quantum;
                    printf(" %s |", p[i].name);
                } else {
                    time += p[i].remaining;
                    p[i].waiting = time - p[i].burst - p[i].arrival;
                    p[i].turnaround = time - p[i].arrival;
                    p[i].remaining = 0;
                    printf(" %s |", p[i].name);

                    totalWT += p[i].waiting;
                    totalTAT += p[i].turnaround;
                    completed++;
                }
            }
        }

        if (done == 1)
            break;
    }

    printf("\n_____________________________________\n0");

    // Display rough time marks (not exact per segment but for visualization)
    printf("\t%d\n", time);

    printf("\nAverage Waiting Time: %.2f", totalWT / n);
    printf("\nAverage Turnaround Time: %.2f\n", totalTAT / n);
}

// ---------- Main ----------
int main() {
    int n, choice, quantum = 2;
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
        printf("\n\nMenu:\n1. FCFS\n2. Round Robin (Quantum = %d)\n3. Exit\nEnter your choice: ", quantum);
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                FCFS(p, n);
                break;
            case 2:
                RoundRobin(p, n, quantum);
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
