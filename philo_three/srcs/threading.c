/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threading.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/07 14:30:45 by ashishae          #+#    #+#             */
/*   Updated: 2020/06/26 18:33:37 by ashishae         ###   ########.fr       */
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

int		threading(t_briefcase proto)
{
	pid_t		*processes;
	t_briefcase	**briefcases;
	int			i;
	int			status;
	// int			pid;

	// proto.death_flag = &death_flag;
	if (init_semaphores(&proto) == -1)
		return (-1);
	init_arrays(&proto);
	briefcases = malloc(sizeof(t_briefcase *) * proto.total);
	processes = malloc(sizeof(pid_t) * (proto.total * 2));
	init_threads(&proto, briefcases);
	i = 0;
	// wait(NULL);
	while(1)
	{
		i = 0;
		status = -42;
		while (i < proto.total)
		{
			waitpid(proto.processes[i], &status, WNOHANG);
			if (status != -42)
			{
				// printf("NONZERO STATUS\n");
				kill_all_processes(proto.total, proto.processes);
				exit(0);
			}
			i++;
		}
		
	}
	
	printf("Exiting");
	return (0);
}
