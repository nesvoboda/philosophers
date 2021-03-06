/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structure_management.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/07 14:10:45 by ashishae          #+#    #+#             */
/*   Updated: 2020/07/09 15:15:16 by ashishae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

/*
** init_arrays resets all arrays in the 'briefcase' prototype called proto.
*/

void		init_arrays(t_briefcase *proto)
{
	long	*last_meal;
	int		*meal_counts;
	int		i;

	last_meal = malloc(sizeof(long) * proto->total);
	meal_counts = malloc(sizeof(int) * proto->total);
	last_meal[0] = get_time();
	i = 0;
	while (i < proto->total)
	{
		last_meal[i] = last_meal[0];
		meal_counts[i] = 0;
		i++;
	}
	proto->lastmeal = last_meal;
	proto->meal_counts = meal_counts;
}

/*
** give_briefcase creates a new 'briefcase' from the prototype proto.
*/

t_briefcase	*give_briefcase(int number, t_briefcase proto)
{
	t_briefcase *new_briefcase;

	new_briefcase = malloc(sizeof(t_briefcase));
	new_briefcase->time_to_sleep = proto.time_to_sleep;
	new_briefcase->time_to_eat = proto.time_to_eat;
	new_briefcase->time_to_die = proto.time_to_die;
	new_briefcase->eat_target = proto.eat_target;
	new_briefcase->total = proto.total;
	new_briefcase->forks = proto.forks;
	new_briefcase->lastmeal = proto.lastmeal;
	new_briefcase->number = number;
	new_briefcase->print = proto.print;
	new_briefcase->death_flag = proto.death_flag;
	new_briefcase->meal_counts = proto.meal_counts;
	new_briefcase->protectors = proto.protectors;
	return (new_briefcase);
}

/*
** init_mutexes creates mutexes in proto (the prototype of all future)
** 'briefcases'.
*/

void		init_mutexes(t_briefcase *proto)
{
	int				i;

	i = 0;
	proto->print = malloc(sizeof(pthread_mutex_t));
	pthread_mutex_init(proto->print, NULL);
	proto->forks = malloc(sizeof(pthread_mutex_t*) * (proto->total));
	proto->protectors = malloc(sizeof(pthread_mutex_t*) * (proto->total));
	while (i < proto->total)
	{
		proto->forks[i] = malloc(sizeof(pthread_mutex_t));
		proto->protectors[i] = malloc(sizeof(pthread_mutex_t));
		pthread_mutex_init(proto->forks[i], NULL);
		pthread_mutex_init(proto->protectors[i], NULL);
		i++;
	}
}
