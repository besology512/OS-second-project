#include <stdio.h>
#include <stdlib.h>
#include "headers.h"

// Define the maximum size of the priority queue
#define MAX_SIZE 100

// Structure to represent an element in the priority queue
struct PriorityQueueElement {
    struct processData value;
    int priority;
};

// Structure to represent the priority queue
struct PriorityQueue {
    struct PriorityQueueElement elements[MAX_SIZE];
    int size;
};

// Function to initialize an empty priority queue
void initializePriorityQueue(struct PriorityQueue *pq) {
    pq->size = 0;
}

// Function to check if the priority queue is empty
int isEmpty(struct PriorityQueue *pq) {
    return pq->size == 0;
}

// Function to check if the priority queue is full
int isFull(struct PriorityQueue *pq) {
    return pq->size == MAX_SIZE;
}

// Function to enqueue an element with its priority
void enqueue(struct PriorityQueue *pq, struct processData value, int priority) {
    if (isFull(pq)) {
        printf("Priority Queue is full. Cannot enqueue.\n");
        return;
    }

    // Create a new element
    struct PriorityQueueElement newElement;
    newElement.value = value;
    newElement.priority = priority;

    // Find the correct position to insert the new element based on priority
    int i;
    for (i = pq->size - 1; i >= 0 && pq->elements[i].priority < priority; i--) {
        pq->elements[i + 1] = pq->elements[i];
    }

    // Insert the new element at the correct position
    pq->elements[i + 1] = newElement;
    pq->size++;
}

// Function to dequeue the element with the highest priority
struct processData dequeue(struct PriorityQueue *pq) {

    struct processData dequeuedValue = pq->elements[0].value;

    // Shift the elements to fill the gap left by the dequeued element
    for (int i = 1; i < pq->size; i++) {
        pq->elements[i - 1] = pq->elements[i];
    }

    pq->size--;

    return dequeuedValue;
}

// Function to print the elements of the priority queue
void printPriorityQueue(struct PriorityQueue *pq) {
    if (isEmpty(pq)) {
        printf("Priority Queue is empty.\n");
        return;
    }

    printf("Priority Queue:\n");
    for (int i = 0; i < pq->size; i++) {
        printf("(%d, Priority: %d) ", pq->elements[i].value.id, pq->elements[i].priority);
    }
    printf("\n");
}
