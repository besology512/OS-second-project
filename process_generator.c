#include "headers.h"

void clearResources(int);

struct processData
{
    int id;
    int arrival;
    int runtime;
    int priority;
};

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

    if (chosenAlgorithm == 1) // Round Robin
    {
        printf("Enter the time step for Round Robin: ");
        scanf("%d", &timeStep);

        // TODO: Handle Round Robin logic with the specified time step
    }
    else if (chosenAlgorithm == 2) // Shortest Job First
    {
        // TODO: Handle Shortest Job First logic
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
    // 3. Initiate and create the scheduler and clock processes.
    // 4. Use this function after creating the clock process to initialize clock
    initClk();
    // To get time use this
    int x = getClk();
    printf("current time is %d\n", x);
    // TODO Generation Main Loop
    // 5. Create a data structure for processes and provide it with its parameters.
    // 6. Send the information to the scheduler at the appropriate time.
    // 7. Clear clock resources
    destroyClk(true);
}

void clearResources(int signum)
{
    // TODO Clears all resources in case of interruption
}
