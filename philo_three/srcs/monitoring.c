/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitoring.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/07 14:29:49 by ashishae          #+#    #+#             */
/*   Updated: 2020/06/26 18:46:28 by ashishae         ###   ########.fr       */
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
	// sem_wait(monitor->death_flag);
	print_state("died", monitor->number, monitor->print);
	sem_wait(monitor->print);
	// kill(monitor->processes[monitor->number], SIGTERM);
	exit(0);
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
		// printf("%d monitored\n", monitor->number);
		diff = now - monitor->last_meal;
		// printf("meal count: %d, meal_target: %d\n", monitor->meal_count, monitor->eat_target);
		if (diff > monitor->time_to_die)
		{
			// printf("M%d: last_meal %ld\n", monitor->number, monitor->last_meal-g_time_start);
			// printf("%d died\n", monitor->number);
			proclaim_death(monitor);
			exit(0);
		}
		usleep(5000);
	}
	// return (NULL);
}
