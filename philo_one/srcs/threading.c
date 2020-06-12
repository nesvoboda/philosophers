/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threading.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/07 14:30:45 by ashishae          #+#    #+#             */
/*   Updated: 2020/06/12 18:47:09 by ashishae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

void init_threads(t_briefcase proto)
{
	pthread_t	*thread_group;
	pthread_t	*monitoring_threads;
	t_briefcase **briefcases;
	int			i;

	i = 0;
	briefcases = malloc(sizeof(t_briefcase *) * proto.total);
	for (int j = 0; j < proto.total; j++)
	{
		briefcases[j] = give_briefcase(j, proto);
	}
	thread_group = malloc(sizeof(pthread_t) * proto.total);
	monitoring_threads = malloc(sizeof(pthread_t) * proto.total);
	while (i < proto.total)
	{
		pthread_create(&thread_group[i], NULL, philosopher, briefcases[i]);
		usleep(10);
		i++;
	}
	i = 0;
	while (i < proto.total)
	{
		pthread_create(&monitoring_threads[i], NULL, monitoring_thread, briefcases[i]);
		// usleep(10);
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
				break;
			i++;
		}
		if (i == proto.total)
			return (1);
	}
	return (0);
}

int threading (t_briefcase proto)
{
	int			death_flag;

	death_flag = -1;
	proto.death_flag = &death_flag;
	// printf("Parameters: %d %d %d %d\n", proto.total, proto.time_to_die, proto.time_to_eat, proto.time_to_sleep);
	init_mutexes(&proto);
	init_arrays(&proto);
	init_threads(proto);
	
	while (1)
	{
		if (check_exit_conditions(proto, death_flag))
			return (0);
		usleep(5);

	}
	return 0;
}
