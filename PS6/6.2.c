/*
===============================================================================
SPPU OS LAB Q6.2 - Page Replacement Algorithms
Algorithms: Optimal (OPT) and Least Recently Used (LRU)
Reference String: 1, 2, 3, 4, 1, 2, 5, 1, 1, 2, 3, 4, 5
===============================================================================
*/

#include <stdio.h>

#define REF_SIZE 13
int reference[REF_SIZE] = {1, 2, 3, 4, 1, 2, 5, 1, 1, 2, 3, 4, 5};

// ---------- Utility Function ----------
int isInFrame(int frames[], int n, int page) {
    for (int i = 0; i < n; i++)
        if (frames[i] == page)
            return 1;
    return 0;
}

// ---------- LRU Page Replacement ----------
void LRU(int framesCount) {
    int frames[10], recent[10];
    int pageFaults = 0;

    for (int i = 0; i < framesCount; i++) {
        frames[i] = -1;
        recent[i] = 0;
    }

    printf("\n===== LRU (Frame Size = %d) =====\n", framesCount);

    for (int i = 0; i < REF_SIZE; i++) {
        int page = reference[i];
        int found = 0;

        // If page already in frame
        for (int j = 0; j < framesCount; j++) {
            if (frames[j] == page) {
                found = 1;
                recent[j] = i + 1; // update recency
                break;
            }
        }

        if (!found) {
            int pos = -1, min = 9999;
            // Find least recently used page
            for (int j = 0; j < framesCount; j++) {
                if (frames[j] == -1) {
                    pos = j;
                    break;
                }
                if (recent[j] < min) {
                    min = recent[j];
                    pos = j;
                }
            }
            frames[pos] = page;
            recent[pos] = i + 1;
            pageFaults++;
        }

        printf("Step %2d: ", i + 1);
        for (int k = 0; k < framesCount; k++) {
            if (frames[k] == -1)
                printf("- ");
            else
                printf("%d ", frames[k]);
        }
        printf(found ? "✅ (Hit)\n" : "❌ (Fault)\n");
    }

    printf("\nTotal Page Faults (LRU, Frames=%d): %d\n", framesCount, pageFaults);
}

// ---------- Optimal Page Replacement ----------
void Optimal(int framesCount) {
    int frames[10];
    int pageFaults = 0;

    for (int i = 0; i < framesCount; i++)
        frames[i] = -1;

    printf("\n===== OPTIMAL (Frame Size = %d) =====\n", framesCount);

    for (int i = 0; i < REF_SIZE; i++) {
        int page = reference[i];
        int found = isInFrame(frames, framesCount, page);

        if (!found) {
            int pos = -1, farthest = i + 1, idx = -1;

            // If an empty slot is available
            for (int j = 0; j < framesCount; j++) {
                if (frames[j] == -1) {
                    pos = j;
                    break;
                }
            }

            // If no empty frame, replace page that is used farthest in future
            if (pos == -1) {
                int farthestDist = -1;
                for (int j = 0; j < framesCount; j++) {
                    int nextUse = -1;
                    for (int k = i + 1; k < REF_SIZE; k++) {
                        if (frames[j] == reference[k]) {
                            nextUse = k;
                            break;
                        }
                    }
                    if (nextUse == -1) {
                        pos = j; // never used again
                        break;
                    }
                    if (nextUse > farthestDist) {
                        farthestDist = nextUse;
                        pos = j;
                    }
                }
            }

            frames[pos] = page;
            pageFaults++;
        }

        printf("Step %2d: ", i + 1);
        for (int k = 0; k < framesCount; k++) {
            if (frames[k] == -1)
                printf("- ");
            else
                printf("%d ", frames[k]);
        }
        printf(found ? "✅ (Hit)\n" : "❌ (Fault)\n");
    }

    printf("\nTotal Page Faults (OPT, Frames=%d): %d\n", framesCount, pageFaults);
}

// ---------- Main Function ----------
int main() {
    printf("SPPU OS LAB Q6.2 - Page Replacement Algorithms\n");
    printf("Reference String: ");
    for (int i = 0; i < REF_SIZE; i++)
        printf("%d ", reference[i]);
    printf("\n");

    // For frame size = 3
    Optimal(3);
    LRU(3);

    // For frame size = 4
    Optimal(4);
    LRU(4);

    return 0;
}
