#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PLATFORMS 5
#define MAX_WAIT 20

// Train structure
typedef struct {
    int id;
    char name[50];
    int priority;      
    int type;          // 1=Express, 2=Passenger, 3=Goods, 4=Superfast
    int arrivalHour;
    int arrivalMin;
    int serviceHour;
    int serviceMin;
    int length;
} Train;

// Platform structure
typedef struct {
    Train train;
    int occupied;
} Platform;

// Globals
Platform platforms[MAX_PLATFORMS];
Train waitingList[MAX_WAIT];
int front = 0, rear = 0;
int nextID = 1;

// Queue helpers
int isWaitingEmpty() { return front == rear; }
int isWaitingFull()  { return rear == MAX_WAIT; }

void enqueue(Train t) {
    if (isWaitingFull()) {
        printf("\nWaiting list FULL! Cannot add %s\n", t.name);
        return;
    }
    waitingList[rear++] = t;
}

Train dequeue() {
    Train t = waitingList[front++];
    if (front == rear) front = rear = 0;
    return t;
}

// Platform helpers
int getFreePlatform() {
    for (int i = 0; i < MAX_PLATFORMS; i++)
        if (!platforms[i].occupied)
            return i;
    return -1;
}

int getLowestPriorityPlatform() {
    int minIndex = 0;
    int minPriority = platforms[0].train.priority;

    for (int i = 1; i < MAX_PLATFORMS; i++) {
        if (platforms[i].train.priority < minPriority) {
            minPriority = platforms[i].train.priority;
            minIndex = i;
        }
    }
    return minIndex;
}

// Train arrival logic
void trainArrival(Train t) {
    int freeIndex = getFreePlatform();

    if (freeIndex != -1) {
        printf("\nPlatform %d assigned to Train %s (ID %d)\n",
               freeIndex + 1, t.name, t.id);
        platforms[freeIndex].train = t;
        platforms[freeIndex].occupied = 1;
        return;
    }

    // No free platform → priority comparison
    int low = getLowestPriorityPlatform();

    if (t.priority > platforms[low].train.priority) {
        printf("\nHigher priority train %s replaces %s on Platform %d\n",
               t.name, platforms[low].train.name, low + 1);

        enqueue(platforms[low].train);
        platforms[low].train = t;

    } else {
        printf("\nAll platforms full. Train %s added to waiting list.\n", t.name);
        enqueue(t);
    }
}

// Train departure
void trainDeparture(int platformNumber) {
    int idx = platformNumber - 1;

    if (idx < 0 || idx >= MAX_PLATFORMS) {
        printf("\nInvalid platform!\n");
        return;
    }

    if (!platforms[idx].occupied) {
        printf("\nPlatform %d already empty.\n", platformNumber);
        return;
    }

    printf("\nTrain %s (ID %d) departed from Platform %d\n",
           platforms[idx].train.name,
           platforms[idx].train.id,
           platformNumber);

    platforms[idx].occupied = 0;

    if (!isWaitingEmpty()) {
        Train next = dequeue();
        printf("Assigning waiting train %s (ID %d) to Platform %d\n",
               next.name, next.id, platformNumber);
        platforms[idx].train = next;
        platforms[idx].occupied = 1;
    }
}

// Display status
void displayStatus() {
    char *typeStr[] = {"", "Express", "Passenger", "Goods", "Superfast"};

    printf("\n========= PLATFORM STATUS =========\n");
    for (int i = 0; i < MAX_PLATFORMS; i++) {
        if (platforms[i].occupied) {
            Train t = platforms[i].train;
            printf("Platform %d: %s (ID %d, %s, P%d, Arr %02d:%02d, Serv %02dh %02dm, Len %d)\n",
                   i + 1, t.name, t.id, typeStr[t.type], t.priority,
                   t.arrivalHour, t.arrivalMin,
                   t.serviceHour, t.serviceMin,
                   t.length);
        } else {
            printf("Platform %d: EMPTY\n", i + 1);
        }
    }

    printf("\n========= WAITING LIST =========\n");
    if (isWaitingEmpty()) {
        printf("Waiting list empty\n");
    } else {
        for (int i = front; i < rear; i++) {
            Train t = waitingList[i];
            printf("%s (ID %d, P%d, Arr %02d:%02d)\n",
                   t.name, t.id, t.priority, t.arrivalHour, t.arrivalMin);
        }
    }
}

// Search function
void searchTrain() {
    char name[50];
    printf("Enter train name to search: ");
    scanf("%s", name);

    // Search platforms
    for (int i = 0; i < MAX_PLATFORMS; i++) {
        if (platforms[i].occupied && strcmp(platforms[i].train.name, name) == 0) {
            printf("\nTrain %s found on Platform %d\n", name, i + 1);
            return;
        }
    }

    // Search waiting list
    for (int i = front; i < rear; i++) {
        if (strcmp(waitingList[i].name, name) == 0) {
            printf("\nTrain %s found in waiting list\n", name);
            return;
        }
    }

    printf("\nTrain %s NOT found\n", name);
}

// System Report
void report() {
    int onPlatforms = 0;
    for (int i = 0; i < MAX_PLATFORMS; i++)
        if (platforms[i].occupied) onPlatforms++;

    int waiting = rear - front;

    printf("\n====== SYSTEM REPORT ======\n");
    printf("Total Platforms: %d\n", MAX_PLATFORMS);
    printf("Trains on platforms: %d\n", onPlatforms);
    printf("Trains in waiting list: %d\n", waiting);
    printf("Next Train ID: %d\n", nextID);
    printf("==========================\n");
}

int main() {
    int choice;

    // Initialize platforms
    for (int i = 0; i < MAX_PLATFORMS; i++)
        platforms[i].occupied = 0;

    while (1) {
        printf("\n====== RAILWAY PLATFORM SIMULATION ======\n");
        printf("1. Train Arrival\n");
        printf("2. Train Departure\n");
        printf("3. Display Status\n");
        printf("4. Search Train\n");
        printf("5. System Report\n");
        printf("6. Exit\n");

        printf("Enter choice: ");
        scanf("%d", &choice);

        if (choice == 1) {
            Train t;
            t.id = nextID++;

            printf("Enter train name: ");
            scanf("%s", t.name);

            printf("Enter priority (1-10): ");
            scanf("%d", &t.priority);

            printf("Enter train type (1-Express, 2-Passenger, 3-Goods, 4-Superfast): ");
            scanf("%d", &t.type);

            printf("Enter arrival hour: ");
            scanf("%d", &t.arrivalHour);

            printf("Enter arrival minute: ");
            scanf("%d", &t.arrivalMin);

            printf("Enter service time hours: ");
            scanf("%d", &t.serviceHour);

            printf("Enter service time minutes: ");
            scanf("%d", &t.serviceMin);

            printf("Enter train length (coaches): ");
            scanf("%d", &t.length);

            trainArrival(t);

        } else if (choice == 2) {
            int p;
            printf("Enter platform number (1-%d): ", MAX_PLATFORMS);
            scanf("%d", &p);
            trainDeparture(p);

        } else if (choice == 3) {
            displayStatus();

        } else if (choice == 4) {
            searchTrain();

        } else if (choice == 5) {
            report();

        } else if (choice == 6) {
            printf("\nExiting simulation...\n");
            break;

        } else {
            printf("\nInvalid choice! Try again.\n");
        }
    }

    return 0;
}
