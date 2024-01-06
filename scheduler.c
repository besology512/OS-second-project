#include "queue.h"
#include "normalQueue.h"

struct Queue normalReadyQueue;
struct PriorityQueue processes;
struct PriorityQueue terminated;
int msgQueue;
struct processData dequeuedVal;
int done = 1;
int StartTime;
int quantum = 3; // Temp value //Time quantum for Round Robin
int TotalSchTime;
FILE *SCHlog;

void SJFAlgo()
{
    if (done == 1 && !isEmpty(&processes))
    {
        dequeuedVal = dequeue(&processes);
        char status[] = "running";
        strcpy(dequeuedVal.status, status);
        int runtime = dequeuedVal.runtime;
        dequeuedVal.remaining_time = dequeuedVal.runtime;
        dequeuedVal.excution_time = dequeuedVal.runtime;
        dequeuedVal.waiting_time = getClk() - dequeuedVal.enteredReady;
        char numberStr[20];
        snprintf(numberStr, sizeof(numberStr), "%d", dequeuedVal.runtime);
        pid_t child_pid = fork();
        if (child_pid == -1)
        {
            printf("error in forking");
        }
        if (child_pid == 0)
        {
            fprintf(SCHlog, "At time %d process %d started arr %d total %d remain %d wait %d", getClk(),
                    dequeuedVal.id, dequeuedVal.arrival, dequeuedVal.runtime, dequeuedVal.remaining_time, dequeuedVal.waiting_time);
            execl("process.out", "child_program", numberStr, NULL);
        }
        else
        {
            done = 0;
            StartTime = getClk();
        }
    }
    else if (done == 1 && isEmpty(&processes))
    {
    }
    else if (done == 0)
    {
        int status;
        if (StartTime - getClk() > 0)
        {
            dequeuedVal.remaining_time--;
            StartTime = getClk();
        }
        pid_t terminated_pid = waitpid(-1, &status, WNOHANG);
        if (terminated_pid > 0)
        {
            dequeuedVal.termination_time = getClk();
            dequeuedVal.TRT = dequeuedVal.termination_time - dequeuedVal.arrival;
            dequeuedVal.WTRT = dequeuedVal.TRT / dequeuedVal.runtime;
            char status[] = "terminated";
            strcpy(dequeuedVal.status, status);
            printf("Ronaldo \n");
            fprintf(SCHlog, "At time %d process %d terminated arr %d total %d remain %d wait %d TA %d WTA %d", getClk(),
                    dequeuedVal.id, dequeuedVal.arrival, dequeuedVal.runtime, 0, dequeuedVal.waiting_time, dequeuedVal.TRT, dequeuedVal.WTRT);
            enqueue(&terminated, dequeuedVal, -dequeuedVal.id);
            done = 1;
        }
    }
}

void RRAlgo()
{
    if (done == 1 && !isNormalQueueEmpty(&normalReadyQueue))
    {
        // printf("normal queue not empty\n");
        dequeuedVal = dequeueNormal(&normalReadyQueue);
        char status[] = "running";
        strcpy(dequeuedVal.status, status);

        dequeuedVal.remaining_time = dequeuedVal.runtime;

        int execution_time = (dequeuedVal.remaining_time > quantum) ? quantum : dequeuedVal.remaining_time;

        dequeuedVal.excution_time = execution_time;
        dequeuedVal.waiting_time = getClk() - dequeuedVal.enteredReady;

        printf("process id: %d remaining time: %d \n", dequeuedVal.id, dequeuedVal.remaining_time); // process id and its remaining time

        char numberStr[20];
        snprintf(numberStr, sizeof(numberStr), "%d", execution_time);

        pid_t child_pid = fork();
        if (child_pid == -1)
        {
            printf("Error in forking\n");
        }

        if (child_pid == 0)
        {
            execl("process.out", "child_program", numberStr, NULL);
        }
        else
        {
            done = 0;
            StartTime = getClk();

            // // Check if the process has remaining time, and enqueue it back if needed
            // if (dequeuedVal.remaining_time > 0) {
            //     enqueueNormal(&normalReadyQueue, dequeuedVal);
            // }
        }
    }
    else if (done == 1 && isNormalQueueEmpty(&normalReadyQueue))
    {
    }
    else if (done == 0)
    {
        int status;

        // THIS IF WILL BE DELETED
        if (StartTime - getClk() > 0)
        {
            dequeuedVal.remaining_time -= 1; // or might make it just = -1
            dequeuedVal.runtime -= 1;
            StartTime = getClk();
            printf("DECREASED\n");
        }

        if (getClk() - StartTime > 0)
        {
            dequeuedVal.remaining_time -= 1; // or might make it just = -1
            dequeuedVal.runtime -= 1;
            StartTime = getClk();
            // printf("DECREASED22\n");
        }

        pid_t terminated_pid = waitpid(-1, &status, WNOHANG);

        // Check if the process has remaining time, and enqueue it back if needed
        if (dequeuedVal.remaining_time > 0 && terminated_pid > 0)
        {
            char status[] = "ready";
            strcpy(dequeuedVal.status, status);
            enqueueNormal(&normalReadyQueue, dequeuedVal);
            done = 1;
        }
        else if (terminated_pid > 0)
        {
            char status[] = "terminated";
            strcpy(dequeuedVal.status, status);
            printf("RONALDO AND MEGA (terminated)\n");
            enqueue(&terminated, dequeuedVal, -dequeuedVal.id);
            done = 1;
        }
    }
}

void receiveProcesses()
{
    struct processData process;

    int rcval = msgrcv(msgQueue, &process, sizeof(struct processData), 0, IPC_NOWAIT);
    if (rcval != -1)
    {
        printf("Received Process: ID=%d, Arrival=%d, Runtime=%d, Priority=%d\n",
               process.id, process.arrival, process.runtime, process.priority);
        // printf("I recieved something\n");
        char status[] = "Ready";
        strcpy(process.status, status);
        process.enteredReady = getClk();
        enqueue(&processes, process, -process.runtime);
        // You can add the received process to your RDY queue or perform any other desired actions
    }
}

void receiveProcessesRR()
{
    struct processData process;

    int rcval = msgrcv(msgQueue, &process, sizeof(struct processData), 0, IPC_NOWAIT);
    if (rcval != -1)
    {
        printf("Received Process: ID=%d, Arrival=%d, Runtime=%d, Priority=%d\n",
               process.id, process.arrival, process.runtime, process.priority);
        // printf("I recieved something");
        char status[] = "Ready";
        strcpy(process.status, status);
        enqueueNormal(&normalReadyQueue, process);
        // You can add the received process to your RDY queue or perform any other desired actions
    }
}
void receiveSchedulerData(struct schedulerData *sch)
{
    int msgQueueKey = ftok(".", 'b'); // Use the same key as in process_generator.c
    int msgQueue = msgget(msgQueueKey, 0666 | IPC_CREAT);
    if (msgQueue == -1)
    {
        perror("Error creating/opening message queue");
        exit(EXIT_FAILURE);
    }

    // Receive the struct schedulerData sch from the message queue
    if (msgrcv(msgQueue, sch, sizeof(struct schedulerData), 0, IPC_NOWAIT) == -1)
    {
        perror("Error receiving message");
        exit(EXIT_FAILURE);
    }
}
void createLogs()
{
    double TOTWTA = 0;
    double TOTWAITING = 0;
    double AVGWTA = 0;
    double AVGWAITING = 0;
    double STDWTA = 0;
    struct processData dequeuedValTerm;
    while (!isEmpty(&terminated))
    {
        dequeuedValTerm = dequeue(&terminated);
        TOTWTA = TOTWTA + dequeuedValTerm.WTRT;
        TOTWAITING = TOTWAITING + dequeuedValTerm.waiting_time;
    }
}

int main(int argc, char *argv[])
{
    struct schedulerData sch;
    // Call the function to receive 'sch' from the message queue
    receiveSchedulerData(&sch);
    // Print received scheduler data for verification
    printf("Received Scheduler Data:\n");
    printf("Number of Processes: %d\n", sch.NProcesses);
    printf("Algorithm Type: %d\n", sch.AlgoType);
    if (sch.AlgoType == 1) // Round Robin
    {
        printf("Time Step for Round Robin: %d\n", sch.Tstep);
    }
    else
    {
        printf("No Time Step (Not Round Robin)\n");
    }
    initClk();
    SCHlog = fopen("log.txt", "w");
    initializePriorityQueue(&processes);
    initializePriorityQueue(&terminated);
    initializeNormalQueue(&normalReadyQueue);

    // Create a message queue key (should match the one used in process_generator.c)
    key_t msgQueueKey = ftok(".", 'a');

    int startTime = getClk();
    // Create or get the message queue
    msgQueue = msgget(msgQueueKey, 0666 | IPC_CREAT);
    if (msgQueue == -1)
    {
        perror("Error creating/opening message queue");
        exit(EXIT_FAILURE);
    }
    while (1)
    {
        // Call the function to receive processes from the message queue
        // receiveProcesses();
        // SJFAlgo();
        receiveProcessesRR();
        RRAlgo();
    }

    int finishTime = getClk();

    TotalSchTime = finishTime - startTime;
    createLogs();
    // Continue with your scheduling logic
    fclose(SCHlog);
    destroyClk(true);
}
