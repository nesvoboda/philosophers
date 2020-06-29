/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/07 14:23:59 by ashishae          #+#    #+#             */
/*   Updated: 2020/06/29 12:43:00 by ashishae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

void	eat(t_briefcase *info)
{
	int number;

	number = info->number;
	sem_wait(info->fork_semaphore);
	sem_wait(info->fork_semaphore);
	sem_wait(info->protectors[number]);
	print_state("has taken a fork", number, info->print);
	print_state("has taken a fork", number, info->print);
	info->last_meal = get_time();
	info->meal_count += 1;
	print_state("is eating", number, info->print);
	usleep(info->time_to_eat * 1000);
	sem_post(info->fork_semaphore);
	sem_post(info->fork_semaphore);
	sem_post(info->protectors[number]);
}

void	*philosopher(void *value)
{
	t_briefcase *info;

	info = value;
	while (1)
	{
		eat(info);
		print_state("is sleeping", info->number, info->print);
		usleep(info->time_to_sleep * 1000);
		print_state("is thinking", info->number, info->print);
	}
}