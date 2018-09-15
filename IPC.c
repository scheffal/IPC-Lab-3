#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h>
#include <signal.h>

void sigHandler(int);
void sigHandlerExit(int);

int main()
{
	pid_t pid, ppid;
	int sleepTime;
	int randSig;

	/*Get parent PID*/
	ppid = getpid();

	/*Install signal handler*/
	signal(SIGUSR1, sigHandler);
	signal(SIGUSR2, sigHandler);
	
	/*Spawn child process*/
	pid = fork();

	if(pid < 0)
	{
		perror("fork failed");
		exit(1);
	}
	else if(!pid)
	{
		printf("Spawned child PID# %d\n", getpid());
		while(1)
		{
			/*Random sleep from 1 to 5 seconds*/
			sleepTime = rand() % 5 + 1;
			printf("Waiting...\t");
			fflush(stdout);
			sleep(sleepTime);

			/*Randomly raise signal*/
			randSig = rand() % 2;
			if(randSig == 0)
			{
				kill(ppid, SIGUSR1);
			}else{
				kill(ppid, SIGUSR2);
			}

			/*First printing gets messed up if this isn't here. Not sure if there is a better way to fix printing*/
			usleep(100);
		}
		
	}else{
		while(1)
		{
			/*Defined user signal here so exit info only prints once*/
			signal(SIGINT, sigHandlerExit);			

			/*Wait for signal to be raised*/
			pause();
		}
	}
	

	return 0;
}

void sigHandlerExit(int sigNum)
{
	printf(" received. Shutting down...\n");
	exit(0);
}

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
