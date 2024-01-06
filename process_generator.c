#include "headers.h"

void clearResources(int);

void sendProcessInfo(int id, int arrival, int runtime, int priority)
{
    // Create a message queue key (you can use ftok or any other method)
    key_t msgQueueKey = ftok(".", 'a');

    // Create or get the message queue
    int msgQueue = msgget(msgQueueKey, 0666 | IPC_CREAT);
    if (msgQueue == -1)
    {
        perror("Error creating/opening message queue");
        exit(EXIT_FAILURE);
    }

    // Prepare the message
    struct processData process;
    process.id = id;
    process.arrival = arrival;
    process.runtime = runtime;
    process.priority = priority;
    char status[] = "new";
    strcpy(process.status, status);

    // Send the message
    if (msgsnd(msgQueue, &process, sizeof(struct processData), 0) == -1)
    {
        perror("Error sending message");
        exit(EXIT_FAILURE);
    }
}

void sendSchedulerData(struct schedulerData *sch)
{
    int msgQueueKey = ftok(".", 'b'); // Use a different key
    int msgQueue = msgget(msgQueueKey, 0666 | IPC_CREAT);
    if (msgQueue == -1)
    {
        perror("Error creating/opening message queue");
        exit(EXIT_FAILURE);
    }

    // Send the struct schedulerData sch to the message queue
    if (msgsnd(msgQueue, sch, sizeof(struct schedulerData), 0) == -1)
    {
        perror("Error sending message");
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char *argv[])
{
    signal(SIGINT, clearResources);

    // TODO Initialization
    // 1. Read the input files.
    FILE *pFile = fopen("processes.txt", "r");
    if (pFile == NULL)
    {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    // Process data from the file, ignoring comments
    char line[100];
    int numProcesses = 0;                    // To keep track of the number of processes
    struct processData *allProcesses = NULL; // Dynamic array to store all processes
    int nextProcessIndex = 0;                // Index of the next process to consider for sending

    while (fgets(line, sizeof(line), pFile))
    {
        if (line[0] != '#')
        {
            // Parse the line and extract process parameters
            int id, arrival, runtime, priority;
            sscanf(line, "%d%d%d%d", &id, &arrival, &runtime, &priority);

            // Increase the size of the dynamic array
            numProcesses++;
            allProcesses = realloc(allProcesses, numProcesses * sizeof(struct processData));

            // Store the process parameters in the dynamic array
            allProcesses[numProcesses - 1].id = id;
            allProcesses[numProcesses - 1].arrival = arrival;
            allProcesses[numProcesses - 1].runtime = runtime;
            allProcesses[numProcesses - 1].priority = priority;
        }
    }
    fclose(pFile);
    // Print the processes to verify
    for (int i = 0; i < numProcesses; i++)
    {
        printf("Process ID: %d, Arrival Time: %d, Runtime: %d, Priority: %d\n",
               allProcesses[i].id, allProcesses[i].arrival, allProcesses[i].runtime, allProcesses[i].priority);
    }
    // TODO: Continue with your initialization, user input, and clock initialization

    // Now, 'allProcesses' contains information about all processes.
    // You can iterate through this array and send each process to the scheduler at the appropriate time.

    // 2. Ask the user for the chosen scheduling algorithm and its parameters, if there are any.
    // TODO 2: Ask the user for the chosen scheduling algorithm and its parameters, if there are any.
    int chosenAlgorithm;
    int timeStep;
    printf("Choose a scheduling algorithm:\n");
    printf("1. Round Robin (RR)\n");
    printf("2. Shortest Job First (SJF)\n");
    printf("Enter the number corresponding to your choice: ");
    scanf("%d", &chosenAlgorithm);
    struct schedulerData sch;
    sch.NProcesses = numProcesses;
    if (chosenAlgorithm == 1) // Round Robin
    {
        printf("Enter the time step for Round Robin: ");
        scanf("%d", &timeStep);
        sch.AlgoType = 1;
        sch.Tstep = timeStep;

        // TODO: Handle Round Robin logic with the specified time step
    }
    else if (chosenAlgorithm == 2) // Shortest Job First
    {
        sch.AlgoType = 2;
    }
    else
    {
        printf("Invalid choice. Exiting.\n");
        exit(EXIT_FAILURE);
    }
    // Verify TODO 2 works
    printf("Chosen Algorithm: %d\n", chosenAlgorithm);
    if (chosenAlgorithm == 1)
    {
        printf("Time Step for Round Robin: %d\n", timeStep);
    }
    // send these data to the to the scheduler
    sendSchedulerData(&sch);
    // TODO: Send the scheduler the necessary information to initialize s
    // 3. Initiate and create the scheduler and clock processes.
    // 4. Use this function after creating the clock process to initialize clock
    initClk();
    // To get time use this
    // TODO Generation Main Loop
    // 5. Create a data structure for processes and provide it with its parameters. )(Done)
    // 6. Send the information to the scheduler at the appropriate time.
    while (nextProcessIndex < numProcesses)
    {
        int x = getClk();
        printf("current time is %d\n", x);
        // Check if the current clock value matches the arrival time of the next process
        if (x == allProcesses[nextProcessIndex].arrival)
        {
            // Send process information
            sendProcessInfo(allProcesses[nextProcessIndex].id,
                            allProcesses[nextProcessIndex].arrival,
                            allProcesses[nextProcessIndex].runtime,
                            allProcesses[nextProcessIndex].priority);

            nextProcessIndex++; // Move to the next process in the array
        }

        // TODO: Add any other clock-related logic or wait mechanism if needed
    }
    // 7. Clear clock resources
    destroyClk(true);
}

void clearResources(int signum)
{
    // TODO Clears all resources in case of interruption
}
