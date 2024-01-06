#define MAX_QUEUE_SIZE 100


struct QueueElement {
    struct processData value;
};

// Structure to represent the queue
struct Queue {
    struct QueueElement elements[MAX_QUEUE_SIZE];
    int front, rear;
};

// Function to initialize an empty queue
void initializeNormalQueue(struct Queue *q) {
    q->front = -1;
    q->rear = -1;
}

// Function to check if the queue is empty
int isNormalQueueEmpty(struct Queue *q) {
    return (q->front == -1 && q->rear == -1);
}

// Function to check if the queue is full
int isNormalQueueFull(struct Queue *q) {
    return (q->rear + 1) % MAX_QUEUE_SIZE == q->front;
}

// Function to enqueue an element
void enqueueNormal(struct Queue *q, struct processData value) {
    if (isNormalQueueFull(q)) {
        printf("Queue is full. Cannot enqueue.\n");
        return;
    }

    if (isNormalQueueEmpty(q)) {
        q->front = 0;
    }

    q->rear = (q->rear + 1) % MAX_QUEUE_SIZE;
    q->elements[q->rear].value = value;
}

// Function to dequeue an element
struct processData dequeueNormal(struct Queue *q) {
    struct processData dequeuedValue;

    if (isNormalQueueEmpty(q)) {
        printf("Queue is empty. Cannot dequeue.\n");
        // You might want to handle this case differently based on your requirements
        // For example, return a special value or terminate the program
        exit(EXIT_FAILURE);
    }

    dequeuedValue = q->elements[q->front].value;

    if (q->front == q->rear) {
        // Last element in the queue is being dequeued
        q->front = -1;
        q->rear = -1;
    } else {
        q->front = (q->front + 1) % MAX_QUEUE_SIZE;
    }

    return dequeuedValue;
}
