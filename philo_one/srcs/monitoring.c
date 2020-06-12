/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitoring.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/07 14:29:49 by ashishae          #+#    #+#             */
/*   Updated: 2020/06/07 17:52:30 by ashishae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

/*
** check_dead goes through all last_meal timestamps and checks if a philosopher
** hasn't had a meal in more than time_to_die miliseconds.
*/

int		check_dead(t_briefcase proto)
{
	int		i;
	long	now;

	now = get_time();
	// printf("Last meals is:\n");
	// for (int y = 0; y < proto.total; y++)
	// 	printf("i: %d, lastmeal: %ld, difference with now: %ld\n", y, proto.lastmeal[y], now - proto.lastmeal[y]);
	i = 0;
	while (i < proto.total)
	{
		if ((int)(now - proto.lastmeal[i]) >= proto.time_to_die)
			return (i);
		i++;
	}
	return (0);

}

void	*monitoring_thread(void *value)
{
	t_briefcase *monitor;
	long	now;
	long 	diff;

	monitor = value;
	now = get_time();
	while (1)
	{

		now = get_time();
		pthread_mutex_lock(monitor->protectors[monitor->number]);
		diff = now - monitor->lastmeal[monitor->number];
		pthread_mutex_unlock(monitor->protectors[monitor->number]);

		if (diff >= monitor->time_to_die)
		{
			if (*(monitor->death_flag) == -1)
			{
				print_state("died", monitor->number, monitor->print,
							monitor->death_flag);
				pthread_mutex_lock(monitor->print);
				*(monitor->death_flag) = monitor->number;
			}
			return (NULL);
		}
		usleep(500);
	}
	return (NULL);
}