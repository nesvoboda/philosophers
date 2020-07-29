/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threading.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/07 14:30:45 by ashishae          #+#    #+#             */
/*   Updated: 2020/07/25 12:37:17 by ashishae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

void	*init_threads(t_briefcase *proto, t_briefcase **briefcases)
{
	int			i;
	pid_t		pid;
	pthread_t	monitor;

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
			briefcases[i]->death_flag = -1;
			pthread_create(&monitor, NULL, monitoring_thread, briefcases[i]);
			philosopher(briefcases[i]);
		}
		proto->processes[i] = pid;
		i++;
		usleep(START_DELAY);
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
	sem_close(info.print);
	sem_close(info.fork_semaphore);
	sem_close(info.grab_semaphore);
	sem_close(info.finish);
	sem_unlink("print");
	sem_unlink("forks");
	sem_unlink("grab");
	sem_unlink("finish");
	name[0] = 'p';
	while (i < info.total)
	{
		sem_close(info.protectors[i]);
		print_long((unsigned long)i, &name[1]);
		sem_unlink(name);
		i++;
	}
}

int		liberate(t_briefcase **briefcases, int total)
{
	int i;

	i = 0;
	free(briefcases[0]->protectors);
	free(briefcases[0]->processes);
	while (i < total)
	{
		free(briefcases[i]);
		i++;
	}
	free(briefcases);
	return (0);
}

int		threading(t_briefcase proto, t_briefcase **briefcases)
{
	if (init_semaphores(&proto) == -1)
		return (-1);
	init_variables(&proto);
	briefcases = malloc(sizeof(t_briefcase *) * proto.total);
	init_threads(&proto, briefcases);
	wait_children(&proto);
	kill_all_processes(proto.total, proto.processes);
	destroy_semaphores(proto);
	exit(liberate(briefcases, proto.total));
	return (0);
}
