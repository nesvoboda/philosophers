/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitoring.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/07 14:29:49 by ashishae          #+#    #+#             */
/*   Updated: 2020/07/25 12:40:22 by ashishae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

/*
** proclaim_death prints out a status message about the death of the
** philosopher the current threads monitors. Then it locks the print
** mutex (to prevent other threads from printing other status messages).
**
** This only happens if another philosopher hasn't died yet
** (and death_flag is set to something other than -1).
**
** Then, death_flag is set to the number of monitored philosopher.
*/

void	proclaim_death(t_briefcase *monitor)
{
	if (*(monitor->death_flag) == -1)
	{
		*(monitor->death_flag) = monitor->number;
		print_state("died", monitor->number + 1, monitor->print,
						monitor->death_flag);
	}
}

int		all_done(t_briefcase *monitor)
{
	int i;

	i = 0;
	while (i < monitor->total)
	{
		if (monitor->meal_counts[i] < monitor->eat_target)
			return (0);
		i++;
	}
	return (1);
}

/*
** monitoring_thread is the function that is run by all monitoring threads.
** It checks if current philosopher has died (more then time_to_die milliseconds
** have elapsed since the time recorded in lastmeal), or any other
** philosopher has died in the meantime (death_flag is not -1). If so, it calls
** proclaim_death and exits.
*/

void	*monitoring_thread(void *value)
{
	t_briefcase	*monitor;
	long		now;
	long		diff;

	monitor = value;
	now = get_time();
	while (1)
	{
		sem_wait(monitor->protectors[monitor->number]);
		now = get_time();
		sem_post(monitor->protectors[monitor->number]);
		diff = now - monitor->lastmeal[monitor->number];
		if (diff > monitor->time_to_die || *(monitor->death_flag) != -1)
		{
			proclaim_death(monitor);
			break ;
		}
		if (monitor->eat_target != -1 && all_done(monitor) == 1)
		{
			*(monitor->death_flag) = 42;
			return (NULL);
		}
		usleep(MONITORING_DELAY);
	}
	return (NULL);
}
