/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threading.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/07 14:30:45 by ashishae          #+#    #+#             */
/*   Updated: 2020/07/11 16:20:58 by ashishae         ###   ########.fr       */
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

int		liberate(pthread_t *thread_group, pthread_t *monitoring_threads,
					t_briefcase **briefcases)
{
	int	total;
	int	i;

	total = briefcases[0]->total;
	free(thread_group);
	free(monitoring_threads);
	free(briefcases[0]->lastmeal);
	free(briefcases[0]->meal_counts);
	free(briefcases[0]->print);
	i = 0;
	while (i < total)
	{
		free(briefcases[0]->forks[i]);
		free(briefcases[0]->protectors[i]);
		i++;
	}
	free(briefcases[0]->forks);
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
	if (total == 1)
		return ;
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
	init_mutexes(&proto);
	init_arrays(&proto);
	briefcases = malloc(sizeof(t_briefcase *) * proto.total);
	thread_group = malloc(sizeof(pthread_t) * proto.total);
	monitoring_threads = malloc(sizeof(pthread_t) * proto.total);
	init_threads(proto, thread_group, monitoring_threads, briefcases);
	wait_children(thread_group, monitoring_threads, proto.total);
	return (liberate(thread_group, monitoring_threads, briefcases));
}
