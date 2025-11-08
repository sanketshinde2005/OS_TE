// 3_3_fcfs_rr_gantt.c
#include <stdio.h>

typedef struct {
    int pid, at, bt, ct, tat, wt;
} Process;

void printGantt(int seq[], int times[], int n) {
    printf("\nGANTT CHART:\n ");
    for(int i=0;i<n;i++) printf("----");
    printf("\n|");
    for(int i=0;i<n;i++) printf(" P%d |",seq[i]);
    printf("\n ");
    for(int i=0;i<n;i++) printf("----");
    printf("\n0");
    for(int i=0;i<n;i++) printf("   %d",times[i]);
    printf("\n");
}

void FCFS(Process p[],int n){
    int t=0,seq[n],times[n];
    for(int i=0;i<n;i++){
        if(t<p[i].at)t=p[i].at;
        t+=p[i].bt;
        p[i].ct=t;
        p[i].tat=p[i].ct-p[i].at;
        p[i].wt=p[i].tat-p[i].bt;
        seq[i]=p[i].pid; times[i]=p[i].ct;
    }
    float awt=0,atat=0;
    printf("\nPID\tAT\tBT\tCT\tTAT\tWT\n");
    for(int i=0;i<n;i++){
        printf("P%d\t%d\t%d\t%d\t%d\t%d\n",p[i].pid,p[i].at,p[i].bt,p[i].ct,p[i].tat,p[i].wt);
        awt+=p[i].wt; atat+=p[i].tat;
    }
    printf("\nAverage WT=%.2f\nAverage TAT=%.2f\n",awt/n,atat/n);
    printGantt(seq,times,n);
}

void RoundRobin(Process p[],int n,int tq){
    int rem[n],done=0,t=0,seq[100],times[100],idx=0;
    for(int i=0;i<n;i++) rem[i]=p[i].bt;

    while(done<n){
        for(int i=0;i<n;i++){
            if(p[i].at<=t && rem[i]>0){
                seq[idx]=p[i].pid;
                if(rem[i]<=tq){
                    t+=rem[i]; rem[i]=0; done++;
                    p[i].ct=t;
                    p[i].tat=p[i].ct-p[i].at;
                    p[i].wt=p[i].tat-p[i].bt;
                }else{
                    rem[i]-=tq; t+=tq;
                }
                times[idx++]=t;
            }
        }
    }

    float awt=0,atat=0;
    printf("\nPID\tAT\tBT\tCT\tTAT\tWT\n");
    for(int i=0;i<n;i++){
        printf("P%d\t%d\t%d\t%d\t%d\t%d\n",p[i].pid,p[i].at,p[i].bt,p[i].ct,p[i].tat,p[i].wt);
        awt+=p[i].wt; atat+=p[i].tat;
    }
    printf("\nAverage WT=%.2f\nAverage TAT=%.2f\n",awt/n,atat/n);
    printGantt(seq,times,idx);
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
    printf("\n1. FCFS\n2. Round Robin (Quantum=2)\n3. Exit\nEnter choice: ");
    scanf("%d",&ch);
    if(ch==1) FCFS(p,n);
    else if(ch==2) RoundRobin(p,n,2);
    else return 0;
}