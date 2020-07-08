/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threading.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/07 14:30:45 by ashishae          #+#    #+#             */
/*   Updated: 2020/07/08 15:39:30 by ashishae         ###   ########.fr       */
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
	int	i;
	int	total;

	i = 0;
	total = briefcases[0]->total;
	free(thread_group);
	free(monitoring_threads);
	free(briefcases);
	return (0);
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
	// while (1)
	// {
	// 	if (check_exit_conditions(proto, death_flag))
	// 		return (liberate(thread_group, monitoring_threads, briefcases));
	// 	usleep(700);
	// }
	for (int i = 0; i < proto.total; i++)
	{
		pthread_join(thread_group[i], NULL);
	}
	for (int i = 0; i < proto.total; i++)
	{
		pthread_join(monitoring_threads[i], NULL);
	}
	return (liberate(thread_group, monitoring_threads, briefcases));
}
