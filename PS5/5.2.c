#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#define MAX 10
void calculateNeed(int P, int R, int need[MAX][MAX], int max[MAX][MAX], int allot[MAX][MAX]) {
    for (int i = 0; i < P; i++)
        for (int j = 0; j < R; j++)
            need[i][j] = max[i][j] - allot[i][j];
}
bool isSafe(int P, int R, int avail[MAX], int max[MAX][MAX], int allot[MAX][MAX]) {
    int need[MAX][MAX];
    calculateNeed(P, R, need, max, allot);
    bool finish[MAX] = {false};
    int safeSeq[MAX];
    int work[MAX];
    for (int i = 0; i < R; i++) work[i] = avail[i];
    int count = 0;
    while (count < P) {
        bool found = false;
        for (int p = 0; p < P; p++) {
            if (!finish[p]) {
                int j;
                for (j = 0; j < R; j++)
                    if (need[p][j] > work[j])
                        break;
                if (j == R) {
                    for (int k = 0; k < R; k++)
                        work[k] += allot[p][k];
                    safeSeq[count++] = p;
                    finish[p] = true;
                    found = true;
                }
            }
        }
        if (!found) {
            printf("\nSystem is NOT in a safe state.\n");
            return false;
        }
    }
    printf("\nSystem is in a SAFE state.\nSafe sequence: ");
    for (int i = 0; i < P; i++) printf("P%d ", safeSeq[i]);
    printf("\n");
    return true;
}
void requestResources(int P, int R, int process, int request[], int avail[MAX], int max[MAX][MAX], int allot[MAX][MAX]) {
    int need[MAX][MAX];
    calculateNeed(P, R, need, max, allot);
    for (int i = 0; i < R; i++)
        if (request[i] > need[process][i]) {
            printf("\nError: Request exceeds process maximum claim.\n");
            return;
        }
    for (int i = 0; i < R; i++)
        if (request[i] > avail[i]) {
            printf("\nProcess P%d must wait: resources not available.\n", process);
            return;
        }
    for (int i = 0; i < R; i++) {
        avail[i] -= request[i];
        allot[process][i] += request[i];
    }
    if (isSafe(P, R, avail, max, allot))
        printf("Request can be GRANTED safely to P%d.\n", process);
    else {
        printf("Request leads to UNSAFE state! Denied.\n");
        for (int i = 0; i < R; i++) {
            avail[i] += request[i];
            allot[process][i] -= request[i];
        }
    }
}
int main() {
    FILE *fp = fopen("state.txt", "r");
    if (!fp) {
        printf("Error: Could not open state.txt\n");
        return 1;
    }
    int P, R;
    fscanf(fp, "Number of Processes %d\n", &P);
    fscanf(fp, "Number of Resources %d\n", &R);
    int allot[MAX][MAX], max[MAX][MAX], avail[MAX];
    printf("\nReading from file...\n");
    printf("Processes: %d | Resources: %d\n", P, R);
    printf("\nAllocation Matrix:\n");
    for (int i = 0; i < P; i++)
        for (int j = 0; j < R; j++)
            fscanf(fp, "%d", &allot[i][j]);
    printf("Max Matrix:\n");
    for (int i = 0; i < P; i++)
        for (int j = 0; j < R; j++)
            fscanf(fp, "%d", &max[i][j]);
    printf("Available Resources:\n");
    for (int i = 0; i < R; i++)
        fscanf(fp, "%d", &avail[i]);
    fclose(fp);
    printf("\n--- Initial Safety Check ---\n");
    isSafe(P, R, avail, max, allot);
    int pid, request[MAX];
    printf("\nEnter process number making request (0–%d): ", P - 1);
    scanf("%d", &pid);
    printf("Enter request vector (%d values): ", R);
    for (int i = 0; i < R; i++) scanf("%d", &request[i]);
    requestResources(P, R, pid, request, avail, max, allot);
    return 0;
}