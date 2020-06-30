#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>

int main()
{
	pid_t pid;
	pid = fork();
	if (pid == 0)
	{
		usleep(500000000);
		printf("EXIT");
	}
	wait(NULL);	
}
