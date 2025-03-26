#include <stdio.h>
#include <limits.h>

#define MAX_FRAMES 10
#define MAX_PAGES 100

// Function declarations
void fifo(int pages[], int n, int frames);
void lru(int pages[], int n, int frames);
void lfu(int pages[], int n, int frames);

int main() {
    int pages[MAX_PAGES], n, frames, choice;

    // Input the number of pages
    printf("Enter the number of pages: ");
    scanf("%d", &n);

    // Input the page reference string
    printf("Enter the page reference string: ");
    for (int i = 0; i < n; i++) {
        scanf("%d", &pages[i]);
    }

    // Input the number of frames
    printf("Enter the number of frames: ");
    scanf("%d", &frames);

    // Menu to choose algorithm
    do {
        printf("\nChoose Page Replacement Algorithm:\n");
        printf("1. FIFO\n2. LRU\n3. LFU\n4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                fifo(pages, n, frames);
                break;
            case 2:
                lru(pages, n, frames);
                break;
            case 3:
                lfu(pages, n, frames);
                break;
            case 4:
                printf("Exiting program...\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 4);

    return 0;
}

////////////////////////////////////////
// FIFO Algorithm Implementation
////////////////////////////////////////
void fifo(int pages[], int n, int frames) {
    int queue[MAX_FRAMES], front = 0, rear = 0;
    int pageFaults = 0, pageHits = 0;
    int isInQueue, i, j;

    for (i = 0; i < frames; i++) {
        queue[i] = -1;  // Initialize frames to -1 (empty)
    }

    for (i = 0; i < n; i++) {
        isInQueue = 0;

        // Check if page is already in the queue
        for (j = 0; j < rear; j++) {
            if (queue[j] == pages[i]) {
                isInQueue = 1;
                pageHits++;
                break;
            }
        }

        // If page is not in the queue, replace the oldest page
        if (!isInQueue) {
            if (rear < frames) {
                queue[rear++] = pages[i];
            } else {
                queue[front] = pages[i];
                front = (front + 1) % frames;
            }
            pageFaults++;
        }

        // Display the queue state
        printf("Queue: ");
        for (j = 0; j < frames; j++) {
            if (queue[j] != -1) {
                printf("%d ", queue[j]);
            } else {
                printf("- ");
            }
        }
        printf("\n");
    }

    printf("\nTotal Page Faults (FIFO): %d\n", pageFaults);
    printf("Total Page Hits (FIFO): %d\n", pageHits);
}

////////////////////////////////////////
// LRU Algorithm Implementation
////////////////////////////////////////
void lru(int pages[], int n, int frames) {
    int memory[MAX_FRAMES], recent[MAX_FRAMES];
    int pageFaults = 0, pageHits = 0;
    int isInMemory, i, j, leastUsed;

    for (i = 0; i < frames; i++) {
        memory[i] = -1;
        recent[i] = -1;
    }

    for (i = 0; i < n; i++) {
        isInMemory = 0;

        // Check if the page is in memory
        for (j = 0; j < frames; j++) {
            if (memory[j] == pages[i]) {
                isInMemory = 1;
                recent[j] = i;
                pageHits++;
                break;
            }
        }

        // If page is not in memory, replace the least recently used page
        if (!isInMemory) {
            if (i < frames) {
                memory[i] = pages[i];
                recent[i] = i;
            } else {
                leastUsed = 0;
                for (j = 1; j < frames; j++) {
                    if (recent[j] < recent[leastUsed]) {
                        leastUsed = j;
                    }
                }
                memory[leastUsed] = pages[i];
                recent[leastUsed] = i;
            }
            pageFaults++;
        }

        // Display the current memory state
        printf("Memory: ");
        for (j = 0; j < frames; j++) {
            if (memory[j] != -1) {
                printf("%d ", memory[j]);
            } else {
                printf("- ");
            }
        }
        printf("\n");
    }

    printf("\nTotal Page Faults (LRU): %d\n", pageFaults);
    printf("Total Page Hits (LRU): %d\n", pageHits);
}

////////////////////////////////////////
// LFU Algorithm Implementation
////////////////////////////////////////
void lfu(int pages[], int n, int frames) {
    int memory[MAX_FRAMES], frequency[MAX_FRAMES];
    int pageFaults = 0, pageHits = 0;
    int isInMemory, i, j, leastFrequent, minFrequency;

    for (i = 0; i < frames; i++) {
        memory[i] = -1;
        frequency[i] = 0;
    }

    for (i = 0; i < n; i++) {
        isInMemory = 0;

        // Check if the page is in memory
        for (j = 0; j < frames; j++) {
            if (memory[j] == pages[i]) {
                isInMemory = 1;
                frequency[j]++;
                pageHits++;
                break;
            }
        }

        // If page is not in memory, replace the least frequently used
        if (!isInMemory) {
            if (i < frames) {
                memory[i] = pages[i];
                frequency[i] = 1;
            } else {
                leastFrequent = 0;
                minFrequency = frequency[0];
                for (j = 1; j < frames; j++) {
                    if (frequency[j] < minFrequency) {
                        leastFrequent = j;
                        minFrequency = frequency[j];
                    }
                }
                memory[leastFrequent] = pages[i];
                frequency[leastFrequent] = 1;
            }
            pageFaults++;
        }

        // Display the current memory state
        printf("Memory: ");
        for (j = 0; j < frames; j++) {
            if (memory[j] != -1) {
                printf("%d ", memory[j]);
            } else {
                printf("- ");
            }
        }
        printf("\n");
    }

    printf("\nTotal Page Faults (LFU): %d\n", pageFaults);
    printf("Total Page Hits (LFU): %d\n", pageHits);
}
