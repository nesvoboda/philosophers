/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/07 14:23:59 by ashishae          #+#    #+#             */
/*   Updated: 2020/06/18 18:33:36 by ashishae         ###   ########.fr       */
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
	print_state("has taken a fork", number, info->print, info->death_flag);
	print_state("has taken a fork", number, info->print, info->death_flag);
	info->lastmeal[number] = get_time();
	info->meal_counts[number] += 1;
	print_state("is eating", number, info->print, info->death_flag);
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
		// printf("Forks semaphore is %p\n", info->fork_semaphore);
		eat(info);
		print_state("is sleeping", info->number, info->print, info->death_flag);
		usleep(info->time_to_sleep * 1000);
		print_state("is thinking", info->number, info->print, info->death_flag);
	}
	return (NULL);
}
