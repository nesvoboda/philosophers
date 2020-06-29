/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitoring.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/07 14:29:49 by ashishae          #+#    #+#             */
/*   Updated: 2020/06/29 12:42:39 by ashishae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

/*
** proclaim_death prints out a status message about the death of the
** philosopher the current threads monitors. Then it posts to the print
** semaphore (to prevent other threads from printing other status messages).
**
*/

void	proclaim_death(t_briefcase *monitor)
{
	print_state("died", monitor->number, monitor->print);
	sem_wait(monitor->print);
	exit(0);
}

/*
** monitoring_thread is the function that is run by all monitoring threads.
** It checks if current philosopher has died (more then time_to_die milliseconds
** have elapsed since the time recorded in lastmeal). If so,
** monitoring thread exits as 0 (and kills the whole philosopher-monitor
** process). The main process receives its signal and kills all other processes.
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
		diff = now - monitor->last_meal;
		if (diff > monitor->time_to_die)
		{
			proclaim_death(monitor);
			exit(0);
		}
		if (monitor->eat_target > 0 &&
				monitor->meal_count >= monitor->eat_target)
			exit(1);
		usleep(500);
	}
}
