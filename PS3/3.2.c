// 3_2_fcfs_sjf_preemptive_gantt.c
#include <stdio.h>

typedef struct {
    int pid, at, bt, rt, ct, tat, wt, completed;
} Process;

void printGantt(int seq[], int times[], int count) {
    printf("\nGANTT CHART:\n ");
    for (int i = 0; i < count; i++) printf("----");
    printf("\n|");
    for (int i = 0; i < count; i++) printf(" P%d |", seq[i]);
    printf("\n ");
    for (int i = 0; i < count; i++) printf("----");
    printf("\n0");
    for (int i = 0; i < count; i++) printf("   %d", times[i]);
    printf("\n");
}

void FCFS(Process p[], int n) {
    int t = 0, seq[n], times[n];
    for (int i = 0; i < n; i++) {
        if (t < p[i].at) t = p[i].at;
        t += p[i].bt;
        p[i].ct = t;
        p[i].tat = p[i].ct - p[i].at;
        p[i].wt = p[i].tat - p[i].bt;
        seq[i] = p[i].pid;
        times[i] = p[i].ct;
    }

    float awt=0,atat=0;
    printf("\nPID\tAT\tBT\tCT\tTAT\tWT\n");
    for(int i=0;i<n;i++){
        printf("P%d\t%d\t%d\t%d\t%d\t%d\n",
               p[i].pid,p[i].at,p[i].bt,p[i].ct,p[i].tat,p[i].wt);
        awt+=p[i].wt; atat+=p[i].tat;
    }
    printf("\nAverage WT=%.2f\nAverage TAT=%.2f\n",awt/n,atat/n);
    printGantt(seq,times,n);
}

void SJF_Preemptive(Process p[], int n) {
    int completed=0, time=0, seq[100], times[100], count=0, prev=-1;
    for(int i=0;i<n;i++){p[i].rt=p[i].bt; p[i].completed=0;}

    while(completed<n){
        int idx=-1,min=9999;
        for(int i=0;i<n;i++)
            if(p[i].at<=time && !p[i].completed && p[i].rt<min && p[i].rt>0){
                idx=i; min=p[i].rt;
            }

        if(idx!=-1){
            if(prev!=idx){seq[count]=p[idx].pid; times[count++]=time; prev=idx;}
            p[idx].rt--; time++;
            if(p[idx].rt==0){
                p[idx].completed=1;
                p[idx].ct=time;
                p[idx].tat=p[idx].ct-p[idx].at;
                p[idx].wt=p[idx].tat-p[idx].bt;
                completed++;
            }
        }else time++;
    }
    times[count]=time;

    float awt=0,atat=0;
    printf("\nPID\tAT\tBT\tCT\tTAT\tWT\n");
    for(int i=0;i<n;i++){
        printf("P%d\t%d\t%d\t%d\t%d\t%d\n",
               p[i].pid,p[i].at,p[i].bt,p[i].ct,p[i].tat,p[i].wt);
        awt+=p[i].wt; atat+=p[i].tat;
    }
    printf("\nAverage WT=%.2f\nAverage TAT=%.2f\n",awt/n,atat/n);
    printGantt(seq,times,count);
}

int main(){
    int n,ch;
    printf("Enter number of processes: ");
    scanf("%d",&n);
    Process p[n];
    printf("Enter Arrival & Burst times:\n");
    for(int i=0;i<n;i++){
        printf("P%d: ",i+1);
        scanf("%d%d",&p[i].at,&p[i].bt);
        p[i].pid=i+1;
    }
    printf("\n1. FCFS\n2. SJF (Preemptive)\n3. Exit\nEnter choice: ");
    scanf("%d",&ch);
    if(ch==1) FCFS(p,n);
    else if(ch==2) SJF_Preemptive(p,n);
    else return 0;
}