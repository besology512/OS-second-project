#include "headers.h"

/* Modify this file as needed*/
int remainingtime;
int runTime;

int main(int agrc, char *argv[])
{
    initClk();
    int x = getClk();
    // TODO it needs to get the remaining time from scheduler
    // remainingtime = ??;
    remainingtime = atoi(argv[1]);

    printf("remaining time in this iteration: ");
    printf("%d \n", remainingtime);
    while (remainingtime > 0 )
    {
        if(getClk() - x >0){
            remainingtime = remainingtime - 1;
            printf("remaining time in this iteration: ");
            printf("%d \n", remainingtime);
            x = getClk();
        }
    }

    destroyClk(false);

    return 0;
}
