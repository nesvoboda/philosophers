/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/06 17:04:19 by ashishae          #+#    #+#             */
/*   Updated: 2020/06/07 14:24:20 by ashishae         ###   ########.fr       */
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
		// printf("Diff: %ld\n", diff);
		pthread_mutex_unlock(monitor->protectors[monitor->number]);

		if (diff >= monitor->time_to_die)
		{
			if (*(monitor->death_flag) == 0)
			{
				print_state("died", monitor->number, monitor->print);
				pthread_mutex_lock(monitor->print);
				*(monitor->death_flag) = monitor->number;
			}
			return (NULL);
		}
		usleep(8);
	}
	return (NULL);
}

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
		usleep(5);
		i++;
	}
	// i = 0;
	// while (i < proto.total)
	// {
	// 	pthread_create(&monitoring_threads[i], NULL, monitoring_thread, briefcases[i]);
	// 	i++;
	// }
}

int		check_exit_conditions(t_briefcase proto, int death_flag)
{
	int i;

	i = 0;
	if (death_flag != 0)
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

	death_flag = 0;
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

int	set_parameters(int ac, char **av, t_briefcase *proto)
{
	if (ac < 5)
		return (-1);
	proto->total = ft_atoi(av[1]);
	proto->time_to_die = ft_atoi(av[2]);
	proto->time_to_eat = ft_atoi(av[3]);
	proto->time_to_sleep = ft_atoi(av[4]);
	if (ac > 5)
		proto->eat_target = ft_atoi(av[5]);
	else
		proto->eat_target = -1;
	return (0);
}

int main(int ac, char **av)
{
	t_briefcase proto;

	if (set_parameters(ac, av, &proto) < 0)
		return (0);
	return (threading(proto));
}