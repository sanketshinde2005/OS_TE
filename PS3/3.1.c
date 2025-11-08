// 3_1_fcfs_sjf_np_gantt.c
#include <stdio.h>

typedef struct {
    int pid, at, bt, ct, tat, wt, completed;
} Process;

void printGantt(int seq[], int times[], int n) {
    printf("\nGANTT CHART:\n ");
    for (int i = 0; i < n; i++) printf("----");
    printf("\n|");
    for (int i = 0; i < n; i++) printf(" P%d |", seq[i]);
    printf("\n ");
    for (int i = 0; i < n; i++) printf("----");
    printf("\n0");
    for (int i = 0; i < n; i++) printf("   %d", times[i]);
    printf("\n");
}

void FCFS(Process p[], int n) {
    int seq[n], times[n];
    int t = 0;
    for (int i = 0; i < n; i++) {
        if (t < p[i].at) t = p[i].at;
        t += p[i].bt;
        p[i].ct = t;
        p[i].tat = p[i].ct - p[i].at;
        p[i].wt = p[i].tat - p[i].bt;
        seq[i] = p[i].pid;
        times[i] = p[i].ct;
    }

    printf("\nPID\tAT\tBT\tCT\tTAT\tWT\n");
    float awt = 0, atat = 0;
    for (int i = 0; i < n; i++) {
        printf("P%d\t%d\t%d\t%d\t%d\t%d\n",
               p[i].pid, p[i].at, p[i].bt, p[i].ct, p[i].tat, p[i].wt);
        awt += p[i].wt; atat += p[i].tat;
    }
    printf("\nAverage WT = %.2f", awt/n);
    printf("\nAverage TAT = %.2f\n", atat/n);
    printGantt(seq, times, n);
}

void SJF_NP(Process p[], int n) {
    int completed = 0, t = 0;
    int seq[n], times[n], sidx = 0;

    for (int i = 0; i < n; i++) p[i].completed = 0;

    while (completed < n) {
        int idx = -1, min_bt = 9999;
        for (int i = 0; i < n; i++)
            if (p[i].at <= t && !p[i].completed && p[i].bt < min_bt) {
                min_bt = p[i].bt; idx = i;
            }
        if (idx != -1) {
            t += p[idx].bt;
            p[idx].ct = t;
            p[idx].tat = p[idx].ct - p[idx].at;
            p[idx].wt = p[idx].tat - p[idx].bt;
            p[idx].completed = 1;
            seq[sidx] = p[idx].pid;
            times[sidx++] = t;
            completed++;
        } else t++;
    }

    printf("\nPID\tAT\tBT\tCT\tTAT\tWT\n");
    float awt = 0, atat = 0;
    for (int i = 0; i < n; i++) {
        printf("P%d\t%d\t%d\t%d\t%d\t%d\n",
               p[i].pid, p[i].at, p[i].bt, p[i].ct, p[i].tat, p[i].wt);
        awt += p[i].wt; atat += p[i].tat;
    }
    printf("\nAverage WT = %.2f", awt/n);
    printf("\nAverage TAT = %.2f\n", atat/n);
    printGantt(seq, times, n);
}

int main() {
    int n, ch;
    printf("Enter number of processes: ");
    scanf("%d", &n);
    Process p[n];
    printf("Enter Arrival & Burst times:\n");
    for (int i = 0; i < n; i++) {
        printf("P%d: ", i+1);
        scanf("%d%d", &p[i].at, &p[i].bt);
        p[i].pid = i+1;
    }

    printf("\n1. FCFS\n2. SJF (Non-Preemptive)\n3. Exit\nEnter choice: ");
    scanf("%d", &ch);
    if (ch == 1) FCFS(p, n);
    else if (ch == 2) SJF_NP(p, n);
    else return 0;
}