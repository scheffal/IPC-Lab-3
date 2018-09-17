#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h>
#include <signal.h>

void sigHandler(int sigNum)
{
	if(sigNum == SIGUSR1)
	{
		printf("received a SIGUSR1 signal\n");
	}
	if(sigNum == SIGUSR2)
	{
		printf("received a SIGUSR2 signal\n");
	}
}

void sigHandlerExit(int sigNum)
{
	printf(" received. Shutting down...\n");
	exit(0);
}

int main() {

    pid_t pid, pid2;
	int sleepTime;
	int randSig;

    // need to do this for each sigusr
    // then install signal
    struct sigaction act;

    act.sa_handler = sigHandler;

    sigaction(SIGUSR1, &act, NULL);
    sigaction(SIGUSR2, &act, NULL);

    // fork X2
    if ((pid = fork()) < 0) {
        perror("Fork 1 failed");
        exit(1);
    } 
    if (pid) { // parent process
        if (pid2 = fork() < 0) {
            perror("Fork 2 failed");
            exit(1);
        }

        while (1) {
            struct sigaction act2;
            act2.sa_handler = sigHandlerExit;
            sigaction(SIGINT, &act2, NULL);
            pause();
        }
    } else {
        int cpid;
        printf("Spawned child PID %d\n", (cpid = getpid()));
        while (1) {
            /*Random sleep from 1 to 5 seconds*/
			sleepTime = rand() % 5 + 1;
			printf("Waiting...\t");
			fflush(stdout);
			sleep(sleepTime);

			/*Randomly raise signal*/
			randSig = rand() % 2;
			if(randSig == 0)
			{
				kill(cpid, SIGUSR1);
			}else{
				kill(cpid, SIGUSR2);
			}
        }
    }

    return 0;
}