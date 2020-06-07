/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structure_management.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/07 14:10:45 by ashishae          #+#    #+#             */
/*   Updated: 2020/06/07 14:11:44 by ashishae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

t_monitor *give_monitor(int i, t_briefcase proto)
{
	t_monitor *result;

	result = malloc(sizeof(t_monitor));
	result->protectors = proto.protectors;
	result->lastmeal = proto.lastmeal;
	result->number = i;
	result->time_to_die = proto.time_to_die;
	result->death_flag = proto.death_flag;
	result->print = proto.print;
	result->meal_counts = proto.meal_counts;
	result->eat_target = proto.eat_target;
	return (result);
}

void	init_arrays(t_briefcase *proto)
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

t_briefcase *give_briefcase(int number, t_briefcase proto)
{
	t_briefcase *new_briefcase;
	
	new_briefcase = malloc(sizeof(t_briefcase));
	new_briefcase->time_to_sleep = proto.time_to_sleep;
	new_briefcase->time_to_eat = proto.time_to_eat;
	new_briefcase->time_to_die = proto.time_to_die;
	new_briefcase->total = proto.total;
	new_briefcase->forks = proto.forks;
	new_briefcase->lastmeal = proto.lastmeal;
	new_briefcase->number = number;
	new_briefcase->print = proto.print;
	new_briefcase->death_flag = proto.death_flag;
	new_briefcase->meal_counts = proto.meal_counts;
	new_briefcase->protectors = proto.protectors;
	return new_briefcase;
}

void	init_mutexes(t_briefcase *proto)
{
	pthread_mutex_t	*forks;
	pthread_mutex_t	*protectors;
	pthread_mutex_t	*mtx;
	int				i;

	i = 0;
	mtx = malloc(sizeof(pthread_mutex_t));
	pthread_mutex_init(mtx, NULL);
	proto->print = mtx;
	forks = malloc(sizeof(pthread_mutex_t) * (proto->total));
	protectors = malloc(sizeof(pthread_mutex_t) * (proto->total));
	proto->forks = malloc(sizeof(pthread_mutex_t*) * (proto->total));
	proto->protectors = malloc(sizeof(pthread_mutex_t*) * (proto->total));
	while (i < proto->total)
	{
		pthread_mutex_init(&forks[i], NULL);
		pthread_mutex_init(&protectors[i], NULL);
		proto->forks[i] = &forks[i];
		proto->protectors[i] = &protectors[i];
		i++;
	}
}