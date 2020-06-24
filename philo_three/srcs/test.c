#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

void philosopher(int number)
{
	while (1)
	{
		printf("I'm philosopher number %d\n", number);
		usleep(1000);
	}
}

int main()
{
	int pid;
	int i;
	int total;

	total = 4;
	i = 0;

	while (i < total)
	{
		pid = fork();
		if (pid == 0)
		{
			philosopher(i);
			return (0);
		}

		i++;
	}
	printf("I'm the main thread\n");
	usleep(5000);
}