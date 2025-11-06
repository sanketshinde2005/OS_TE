/*
===============================================================================
SPPU OS LAB Q3.2 - CPU Scheduling Algorithms
Algorithms: FCFS (Non-Preemptive) & SJF (Preemptive)
-------------------------------------------------------------------------------
Calculates:
- Average Waiting Time
- Average Turnaround Time
- Gantt Chart
===============================================================================
*/

#include <stdio.h>
#include <limits.h>

// Structure for process details
struct process {
    char name[5];
    int arrival, burst, remaining;
    int waiting, turnaround, completion;
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

    printf("\n===== FCFS Scheduling =====\n");
    printf("GANTT CHART:\n");
    printf("_____________________________________\n|");

    for (int i = 0; i < n; i++) {
        if (time < p[i].arrival)
            time = p[i].arrival;  // CPU idle
        time += p[i].burst;
        p[i].completion = time;
        p[i].turnaround = p[i].completion - p[i].arrival;
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

// ---------- SJF (Preemptive / SRTF) Scheduling ----------
void SJF_Preemptive(struct process p[], int n) {
    int completed = 0, time = 0, minIndex;
    float totalWT = 0, totalTAT = 0;
    int isCompleted[n];
    for (int i = 0; i < n; i++) {
        isCompleted[i] = 0;
        p[i].remaining = p[i].burst;
    }

    printf("\n===== SJF (Preemptive) Scheduling =====\n");
    printf("GANTT CHART:\n");
    printf("_____________________________________\n|");

    while (completed != n) {
        int idx = -1;
        int minBurst = INT_MAX;

        // Find process with minimum remaining time that has arrived
        for (int i = 0; i < n; i++) {
            if (p[i].arrival <= time && p[i].remaining > 0) {
                if (p[i].remaining < minBurst) {
                    minBurst = p[i].remaining;
                    idx = i;
                }
                // Tie-breaking: earlier arrival
                else if (p[i].remaining == minBurst && p[i].arrival < p[idx].arrival) {
                    idx = i;
                }
            }
        }

        if (idx != -1) {
            // Execute process for 1 unit time
            p[idx].remaining--;
            printf(" %s |", p[idx].name);
            time++;

            // If process completed
            if (p[idx].remaining == 0) {
                p[idx].completion = time;
                p[idx].turnaround = p[idx].completion - p[idx].arrival;
                p[idx].waiting = p[idx].turnaround - p[idx].burst;
                totalWT += p[idx].waiting;
                totalTAT += p[idx].turnaround;
                isCompleted[idx] = 1;
                completed++;
            }
        } else {
            // No process ready, CPU idle
            time++;
        }
    }

    printf("\n_____________________________________\n");
    printf("\nAverage Waiting Time: %.2f", totalWT / n);
    printf("\nAverage Turnaround Time: %.2f\n", totalTAT / n);
}

// ---------- MAIN ----------
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
        printf("\n\nMenu:\n1. FCFS\n2. SJF (Preemptive)\n3. Exit\nEnter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                FCFS(p, n);
                break;
            case 2:
                SJF_Preemptive(p, n);
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
