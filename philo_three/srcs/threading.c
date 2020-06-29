/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threading.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/07 14:30:45 by ashishae          #+#    #+#             */
/*   Updated: 2020/06/29 12:35:52 by ashishae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"
void	kill_all_processes(int total, pid_t *processes);

void		*init_threads(t_briefcase *proto, t_briefcase **briefcases)
{
	int			i;
	pid_t		pid;
	pthread_t	*monitoring_threado;

	i = 0;
	while (i < proto->total)
	{
		briefcases[i] = give_briefcase(i, proto);
		printf("At start, meal_count is %d\n", briefcases[i]->meal_count);
		i++;
	}
	i = 0;
	
	monitoring_threado = malloc(sizeof(pthread_t));
	while (i < proto->total)
	{
		// pthread_create(&thread_group[i], NULL, philosopher, briefcases[i]);
		pid = fork();
		if (pid == 0)
		{
			pthread_create(monitoring_threado, NULL, monitoring_thread, briefcases[i]);
			return (philosopher(briefcases[i]));
		}
		proto->processes[i] = pid;
		// usleep(50);
		i++;
	}

	// i = 0;
	// while (i < proto->total)
	// {
	// 	// pthread_create(&monitoring_threads[i], NULL, monitoring_thread,
	// 		// briefcases[i]);
	// 	pid = fork();
	// 	if (pid == 0)
	// 		return (monitoring_thread(briefcases[i]));
	// 	// processes[i + proto->total] = pid;
	// 	i++;
	// }
	printf("Finishing init_threads\n");
	return (0);
}

int		check_exit_conditions(t_briefcase *proto, int death_flag)
{
	int i;

	i = 0;
	(void) death_flag;
	// printf("Death flag is %d\n", death_flag);
	// if (*(proto->death_flag) != -1)
		// return (1);
	if (proto->eat_target > 0)
	{
		while (i < proto->total)
		{
			if (proto->meal_counts[i] < proto->eat_target)
				break ;
			i++;
		}
		if (i == proto->total)
			return (1);
	}
	return (0);
}

/*
** destroy_semaphores unlinks all the semaphores we used in our program
*/

void	destroy_semaphores(t_briefcase info)
{
	int		i;
	char	name[12];

	i = 0;
	sem_unlink("print");
	sem_unlink("forks");
	name[0] = 'p';
	while (i < info.total)
	{
		print_long((unsigned long)i, &name[1]);
		sem_unlink(name);
		i++;
	}
}

void	kill_all_processes(int total, pid_t *processes)
{
	int i;

	i = 0;
	// printf("Killing processes from %d\n", (int)getpid());
	// for (int j=0; j<total; j++)
	// {
	// 	printf("Processes[%d]: %d\n", j, processes[j]);
	// }
	while (i < total)
	{
		kill(processes[i], SIGKILL);
		i++;
	}
}


int all_finished(int *array, int total)
{
	int i;

	i = 0;
	while (i < total)
	{
		if (array[i] != 1)
			return (0);
		i++;
	}
	return (1);
}

void	wait_children(t_briefcase *proto)
{
	int i;
	int status;
	int *finished;

	finished = malloc(sizeof(int) * proto->total);
	// memset(finished, 0, proto->total);
	// printf("Starting to wait wait_children\n");
	status = 42;
	while (1)
	{
		i = 0;
		// printf("Checking status\n");
		while(i < proto->total)
		{
			waitpid(proto->processes[i], &status, WNOHANG);
			if (WIFEXITED(status))
			{
				if (WEXITSTATUS(status) == 1)
				{
					printf("%d finished as 1\n", i);
					finished[i] = 1;
				}
				else if (WEXITSTATUS(status) == 0)
					return ;
				}
			i++;
		}
		if (all_finished(finished, proto->total))
		{
			kill_all_processes(proto->total, proto->processes);
			return ;
		}
		usleep(50);
	}
}

int		threading(t_briefcase proto)
{
	pid_t		*processes;
	t_briefcase	**briefcases;
	// int			pid;

	// proto.death_flag = &death_flag;
	if (init_semaphores(&proto) == -1)
		return (-1);
	init_arrays(&proto);
	briefcases = malloc(sizeof(t_briefcase *) * proto.total);
	processes = malloc(sizeof(pid_t) * (proto.total * 2));
	init_threads(&proto, briefcases);
	wait_children(&proto);
	printf("Exiting");
	return (0);
}
