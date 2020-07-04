/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threading.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/07 14:30:45 by ashishae          #+#    #+#             */
/*   Updated: 2020/07/04 17:59:25 by ashishae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

void	*init_threads(t_briefcase *proto, t_briefcase **briefcases,
			pthread_t **monitors)
{
	int			i;
	pid_t		pid;

	i = 0;
	while (i < proto->total)
	{
		briefcases[i] = give_briefcase(i, proto);
		i++;
	}
	i = 0;
	while (i < proto->total)
	{
		pid = fork();
		if (pid == 0)
		{
			monitors[i] = malloc(sizeof(pthread_t));
			pthread_create(monitors[i], NULL, monitoring_thread,
				briefcases[i]);
			philosopher(briefcases[i]);
		}
		proto->processes[i] = pid;
		i++;
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

int		liberate(pthread_t **monitors)
{
	free(monitors);
	return (0);
}

int		threading(t_briefcase proto, t_briefcase **briefcases)
{
	pthread_t **monitors;

	monitors = malloc(sizeof(pthread_t) * proto.total);
	if (init_semaphores(&proto) == -1)
		return (-1);
	init_variables(&proto);
	briefcases = malloc(sizeof(t_briefcase *) * proto.total);
	init_threads(&proto, briefcases, monitors);
	wait_children(&proto);
	kill_all_processes(proto.total, proto.processes);
	destroy_semaphores(proto);
	exit(liberate(monitors));
	return (0);
}
