#include <stdio.h>
#include <stdlib.h>
#define MAX 20
void displayFrames(int frames[], int frameSize) {
    printf("\n-------------\n|");
    for (int i = 0; i < frameSize; i++) {
        if (frames[i] == -1)
            printf("   |");
        else
            printf(" %d |", frames[i]);
    }
    printf("\n-------------\n");
}
void optimal(int pages[], int n, int frameSize) {
    int frames[frameSize], faults = 0, hits = 0;
    for (int i = 0; i < frameSize; i++) frames[i] = -1;
    printf("\n--- Optimal Page Replacement (Frame Size = %d) ---\n", frameSize);
    for (int i = 0; i < n; i++) {
        int page = pages[i], flag = 0;
        for (int j = 0; j < frameSize; j++) {
            if (frames[j] == page) {
                hits++;
                flag = 1;
                printf("Page %d → Hit", page);
                break;
            }
        }
        if (!flag) {
            int pos = -1, farthest = i + 1;
            for (int j = 0; j < frameSize; j++) {
                int k;
                for (k = i + 1; k < n; k++)
                    if (frames[j] == pages[k]) break;
                if (k == n) { pos = j; break; }
                if (k > farthest) { farthest = k; pos = j; }
            }
            if (pos == -1) pos = 0;
            frames[pos] = page;
            faults++;
            printf("Page %d → Miss", page);
        }
        displayFrames(frames, frameSize);
    }
    printf("\nTotal Page Faults: %d", faults);
    printf("\nTotal Page Hits: %d", hits);
    printf("\nHit Ratio = %.2f", (float)hits / n);
    printf("\nMiss Ratio = %.2f\n", (float)faults / n);
}
void lru(int pages[], int n, int frameSize) {
    int frames[frameSize], recent[frameSize], time = 0, faults = 0, hits = 0;

    for (int i = 0; i < frameSize; i++) {
        frames[i] = -1;
        recent[i] = 0;
    }
    printf("\n--- LRU Page Replacement (Frame Size = %d) ---\n", frameSize);
    for (int i = 0; i < n; i++) {
        int page = pages[i], flag = 0;
        for (int j = 0; j < frameSize; j++) {
            if (frames[j] == page) {
                hits++;
                recent[j] = ++time;
                flag = 1;
                printf("Page %d → Hit", page);
                break;
            }
        }
        if (!flag) {
            int pos = 0, min = recent[0];
            for (int j = 1; j < frameSize; j++) {
                if (recent[j] < min) {
                    min = recent[j];
                    pos = j;
                }
            }
            frames[pos] = page;
            recent[pos] = ++time;
            faults++;
            printf("Page %d → Miss", page);
        }
        displayFrames(frames, frameSize);
    }
    printf("\nTotal Page Faults: %d", faults);
    printf("\nTotal Page Hits: %d", hits);
    printf("\nHit Ratio = %.2f", (float)hits / n);
    printf("\nMiss Ratio = %.2f\n", (float)faults / n);
}
int main() {
    int pages[] = {1, 2, 3, 4, 1, 2, 5, 1, 1, 2, 3, 4, 5};
    int n = sizeof(pages) / sizeof(pages[0]);
    printf("Fixed Page Reference String:\n");
    for (int i = 0; i < n; i++) printf("%d ", pages[i]);
    printf("\n");
    optimal(pages, n, 3);
    lru(pages, n, 3);
    optimal(pages, n, 4);
    lru(pages, n, 4);
    return 0;
}