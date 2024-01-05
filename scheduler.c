#include "headers.h"

struct processData
{
    int id;
    int arrival;
    int runtime;
    int priority;
};

void receiveProcesses()
{
    // Create a message queue key (should match the one used in process_generator.c)
    key_t msgQueueKey = ftok(".", 'a');

    // Create or get the message queue
    int msgQueue = msgget(msgQueueKey, 0666 | IPC_CREAT);
    if (msgQueue == -1)
    {
        perror("Error creating/opening message queue");
        exit(EXIT_FAILURE);
    }

    // Continue receiving messages in a loop
    while (1)
    {
        struct processData process;

        // Receive a message from the queue
        if (msgrcv(msgQueue, &process, sizeof(struct processData), 0, 0) == -1)
        {
            perror("Error receiving message");
            exit(EXIT_FAILURE);
        }

        // Add the received process to your RDY queue or perform any other desired actions
        // For simplicity, let's just print the received process information
        printf("Received Process: ID=%d, Arrival=%d, Runtime=%d, Priority=%d\n",
               process.id, process.arrival, process.runtime, process.priority);

        // You can add the received process to your RDY queue or perform any other desired actions
    }
}

int main(int argc, char *argv[])
{
    initClk();

    // Call the function to receive processes from the message queue
    receiveProcesses();

    // Continue with your scheduling logic

    destroyClk(true);
}
