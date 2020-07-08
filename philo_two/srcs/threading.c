/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threading.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/07 14:30:45 by ashishae          #+#    #+#             */
/*   Updated: 2020/07/08 16:35:03 by ashishae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

void	init_threads(t_briefcase proto, pthread_t *thread_group,
			pthread_t *monitoring_threads, t_briefcase **briefcases)
{
	int			i;

	i = 0;
	while (i < proto.total)
	{
		briefcases[i] = give_briefcase(i, proto);
		i++;
	}
	i = 0;
	while (i < proto.total)
	{
		pthread_create(&thread_group[i], NULL, philosopher, briefcases[i]);
		usleep(50);
		i++;
	}
	i = 0;
	while (i < proto.total)
	{
		pthread_create(&monitoring_threads[i], NULL, monitoring_thread,
			briefcases[i]);
		i++;
	}
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
	sem_close(info.print);
	sem_close(info.fork_semaphore);
	sem_unlink("print");
	sem_unlink("forks");
	name[0] = 'p';
	while (i < info.total)
	{
		print_long((unsigned long)i, &name[1]);
		sem_close(info.protectors[i]);
		sem_unlink(name);
		i++;
	}
}

int		liberate(pthread_t *thread_group, pthread_t *monitoring_threads,
	t_briefcase **briefcases)
{
	int	i;
	int	total;

	i = 0;
	destroy_semaphores(*briefcases[0]);
	total = briefcases[0]->total;
	free(thread_group);
	free(monitoring_threads);
	free(briefcases[0]->lastmeal);
	free(briefcases[0]->meal_counts);
	free(briefcases[0]->protectors);
	i = 0;
	while (i < total)
		free(briefcases[i++]);
	free(briefcases);
	return (0);
}

void	wait_children(pthread_t *thread_group, pthread_t *monitoring_threads,
						int total)
{
	int	i;

	i = 0;
	while (i < total)
		pthread_join(monitoring_threads[i++], NULL);
	i = 0;
	while (i < total)
		pthread_join(thread_group[i++], NULL);
}

int		threading(t_briefcase proto)
{
	int			death_flag;
	pthread_t	*thread_group;
	pthread_t	*monitoring_threads;
	t_briefcase	**briefcases;

	death_flag = -1;
	proto.death_flag = &death_flag;
	if (init_semaphores(&proto) == -1)
		return (-1);
	init_arrays(&proto);
	briefcases = malloc(sizeof(t_briefcase *) * proto.total);
	thread_group = malloc(sizeof(pthread_t) * proto.total);
	monitoring_threads = malloc(sizeof(pthread_t) * proto.total);
	init_threads(proto, thread_group, monitoring_threads, briefcases);
	wait_children(thread_group, monitoring_threads, proto.total);
	return (liberate(thread_group, monitoring_threads, briefcases));
}
