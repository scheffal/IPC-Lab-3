/*********************************************
* Lab 3: Extra Credit
* 452 - 10
* excred.c
*
* Authors:
* Anthony Blanton
* Alison Scheffler
*
**********************************************/

#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h>
#include <signal.h>
#include <time.h>

void sigHandlerExit(int sigNum)
{
	printf(" received. Shutting down...\n");
	exit(0);
}

/**
 * From https://www.linuxprogrammingblog.com/code-examples/sigaction
 * and the man pages
 */
void sighandler(int signum, siginfo_t *siginfo, void *content) {

    char *sigtype;

    switch (signum) {
        case SIGUSR1: sigtype = "SIGUSR1"; break;
        case SIGUSR2: sigtype = "SIGUSR2"; break;
        default: sigHandlerExit(signum);
    }

    printf("Received a %s from %d\n", sigtype, siginfo->si_pid);

	
}


int main() {

    // seed the random number generator
    srand(time(0));

    // pid of processes
    pid_t pid, pid2, ppid = getpid();

    // register the handlers
    struct sigaction act;
    act.sa_sigaction = sighandler;
    act.sa_flags = SA_SIGINFO;
    sigaction(SIGUSR1, &act, NULL);
    sigaction(SIGUSR2, &act, NULL);
    
    // spawn child processes
    if ((pid = fork()) < 0) {
        perror("Fork 1 failed");
        exit(1);
    }

    if (pid) {
        if ((pid2 = fork()) < 0) {
            perror("Fork 2 failed");
            exit(1);
        }
    }

    if (pid && pid2) {
            // parent
        printf("Spawned child PID# %d\n", pid);
        printf("Spawned child PID# %d\n", pid2);

    }

    // do the signal things
    if (pid && pid2) {
        // parent
        sigaction(SIGINT, &act, NULL);
        while (1) {
            printf("Waiting...\t");
            fflush(stdout);
            pause();
        }

    }

    else if (!pid || (pid && !pid2)) {
        // children
        while (1) {
            // wait a random amount of time between 1 and 5 seconds
            int sleepytime = rand() % 5 + 1;
            sleep(sleepytime);

            // generate a random signal
            int signumb = rand() % 2;
            switch (signumb) {
                case 0: kill(ppid, SIGUSR1); break;
                case 1: kill(ppid, SIGUSR2);
            }
        }
    }
        
    return 0;
}