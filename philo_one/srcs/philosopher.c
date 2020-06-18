/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/07 14:23:59 by ashishae          #+#    #+#             */
/*   Updated: 2020/06/18 12:23:12 by ashishae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

void	eat(t_briefcase *info)
{
	int left;
	int right;
	int number;

	number = info->number;
	left = (number + 1) % info->total;
	right = number;
	pthread_mutex_lock(info->forks[right]);
	print_state("has taken a fork", number, info->print, info->death_flag);
	pthread_mutex_lock(info->forks[left]);
	print_state("has taken a fork", number, info->print, info->death_flag);
	pthread_mutex_lock(info->protectors[number]);
	info->lastmeal[number] = get_time();
	info->meal_counts[number] += 1;
	print_state("is eating", number, info->print, info->death_flag);
	usleep(info->time_to_eat * 1000);
	pthread_mutex_unlock(info->protectors[number]);
	pthread_mutex_unlock(info->forks[right]);
	pthread_mutex_unlock(info->forks[left]);
}

void	*philosopher(void *value)
{
	t_briefcase *info;

	info = value;
	while (1)
	{
		eat(info);
		print_state("is sleeping", info->number, info->print, info->death_flag);
		usleep(info->time_to_sleep * 1000);
		print_state("is thinking", info->number, info->print, info->death_flag);
	}
	return (NULL);
}
