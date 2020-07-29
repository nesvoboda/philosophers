/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitoring.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/07 14:29:49 by ashishae          #+#    #+#             */
/*   Updated: 2020/07/25 12:37:39 by ashishae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

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
			monitor->death_flag = 0;
			sem_wait(monitor->finish);
			sem_wait(monitor->print);
			rogue_print_state("died", monitor->number + 1);
			exit(0);
		}
		usleep(MONITORING_DELAY);
	}
}
