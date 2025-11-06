/*
===============================================================================
SPPU OS LAB Q6.1 - Page Replacement Algorithms
Simulate FIFO and LRU algorithms for random page-reference strings.
===============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_PAGES 20
#define MAX_FRAMES 7

// ---------- Function to generate random page reference string ----------
void generateReferenceString(int pages[], int n) {
    for (int i = 0; i < n; i++) {
        pages[i] = rand() % 10; // pages range from 0 to 9
    }
}

// ---------- Function to print page reference string ----------
void printReferenceString(int pages[], int n) {
    printf("\nPage Reference String:\n");
    for (int i = 0; i < n; i++)
        printf("%d ", pages[i]);
    printf("\n");
}

// ---------- FIFO Page Replacement ----------
int FIFO(int pages[], int n, int frames) {
    int queue[MAX_FRAMES], front = 0, rear = 0, count = 0;
    int pageFaults = 0, found;

    for (int i = 0; i < frames; i++)
        queue[i] = -1;

    printf("\nFIFO Simulation:\n");

    for (int i = 0; i < n; i++) {
        found = 0;
        for (int j = 0; j < frames; j++) {
            if (queue[j] == pages[i]) {
                found = 1;
                break;
            }
        }

        if (!found) {
            queue[rear] = pages[i];
            rear = (rear + 1) % frames;
            pageFaults++;

            printf("Page %d caused a page fault: ", pages[i]);
            for (int k = 0; k < frames; k++) {
                if (queue[k] != -1)
                    printf("%d ", queue[k]);
                else
                    printf("- ");
            }
            printf("\n");
        } else {
            printf("Page %d found in frame. No fault.\n", pages[i]);
        }
    }

    return pageFaults;
}

// ---------- LRU Page Replacement ----------
int LRU(int pages[], int n, int frames) {
    int memory[MAX_FRAMES], recent[MAX_FRAMES];
    int pageFaults = 0;

    for (int i = 0; i < frames; i++) {
        memory[i] = -1;
        recent[i] = 0;
    }

    printf("\nLRU Simulation:\n");

    for (int i = 0; i < n; i++) {
        int page = pages[i];
        int found = 0;

        // Check if page is already in memory
        for (int j = 0; j < frames; j++) {
            if (memory[j] == page) {
                found = 1;
                recent[j] = i + 1; // Update recency
                break;
            }
        }

        if (!found) {
            int pos = -1, min = 9999;
            // Find the least recently used page
            for (int j = 0; j < frames; j++) {
                if (memory[j] == -1) {
                    pos = j;
                    break;
                }
                if (recent[j] < min) {
                    min = recent[j];
                    pos = j;
                }
            }

            memory[pos] = page;
            recent[pos] = i + 1;
            pageFaults++;

            printf("Page %d caused a page fault: ", page);
            for (int k = 0; k < frames; k++) {
                if (memory[k] != -1)
                    printf("%d ", memory[k]);
                else
                    printf("- ");
            }
            printf("\n");
        } else {
            printf("Page %d found in frame. No fault.\n", page);
        }
    }

    return pageFaults;
}

// ---------- Main Function ----------
int main() {
    srand(time(NULL));

    int n, frames;
    int pages[MAX_PAGES];

    printf("Enter number of pages in reference string (<= 20): ");
    scanf("%d", &n);
    printf("Enter number of frames (1 to 7): ");
    scanf("%d", &frames);

    generateReferenceString(pages, n);
    printReferenceString(pages, n);

    int fifoFaults = FIFO(pages, n, frames);
    int lruFaults = LRU(pages, n, frames);

    printf("\n==========================================");
    printf("\nTotal Page Faults (FIFO): %d", fifoFaults);
    printf("\nTotal Page Faults (LRU) : %d", lruFaults);
    printf("\n==========================================\n");

    if (fifoFaults < lruFaults)
        printf("➡️ FIFO performed better (fewer page faults).\n");
    else if (fifoFaults > lruFaults)
        printf("➡️ LRU performed better (fewer page faults).\n");
    else
        printf("➡️ Both algorithms performed equally.\n");

    return 0;
}
