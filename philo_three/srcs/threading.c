/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threading.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/07 14:30:45 by ashishae          #+#    #+#             */
/*   Updated: 2020/06/24 16:57:11 by ashishae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

void		*init_threads(t_briefcase proto, t_briefcase **briefcases, 
	pid_t *processes)
{
	int			i;
	pid_t		pid;

	i = 0;
	while (i < proto.total)
	{
		briefcases[i] = give_briefcase(i, proto);
		i++;
	}
	i = 0;
	while (i < proto.total)
	{
		// pthread_create(&thread_group[i], NULL, philosopher, briefcases[i]);
		pid = fork();
		if (pid == 0)
			return (philosopher(&briefcases[i]));
		proto.processes[i] = pid;
		usleep(50);
		i++;
	}
	i = 0;
	while (i < proto.total)
	{
		// pthread_create(&monitoring_threads[i], NULL, monitoring_thread,
			// briefcases[i]);
		pid = fork();
		if (pid == 0)
			return (monitoring_thread(&briefcases[i]));
		proto.processes[i + proto.total] = pid;
		i++;
	}
	return (0);
}

int		check_exit_conditions(t_briefcase proto, int death_flag)
{
	int i;

	i = 0;
	if (death_flag != -1)
		return (1);
	if (proto.eat_target > 0)
	{
		while (i < proto.total)
		{
			if (proto.meal_counts[i] < proto.eat_target)
				break ;
			i++;
		}
		if (i == proto.total)
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
	while (i < total)
	{
		kill(processes[i], SIGKILL);
		i++;
	}
}

int		threading(t_briefcase proto)
{
	int			death_flag;
	pid_t		*processes;
	t_briefcase	**briefcases;

	death_flag = -1;
	proto.death_flag = &death_flag;
	if (init_semaphores(&proto) == -1)
		return (-1);
	init_arrays(&proto);
	briefcases = malloc(sizeof(t_briefcase *) * proto.total);
	init_threads(proto, briefcases, processes);
	while (1)
	{
		if (check_exit_conditions(proto, death_flag))
		{
			destroy_semaphores(proto);
			return (0);
		}
		usleep(50);
	}
	return (0);
}
