/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/07 14:23:59 by ashishae          #+#    #+#             */
/*   Updated: 2020/07/24 15:04:42 by ashishae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

void	eat(t_briefcase *info)
{
	int number;

	number = info->number;
	sem_wait(info->grab_semaphore);
	sem_wait(info->fork_semaphore);
	print_state("has taken a fork", number + 1, info->print, info->death_flag);
	sem_wait(info->fork_semaphore);
	print_state("has taken a fork", number + 1, info->print, info->death_flag);
	sem_post(info->grab_semaphore);
	sem_wait(info->protectors[number]);
	info->lastmeal[number] = get_time();
	print_state("is eating", number + 1, info->print, info->death_flag);
	if (*info->death_flag == -1)
		ft_sleep(info->time_to_eat);
	info->meal_counts[number] += 1;
	sem_post(info->fork_semaphore);
	sem_post(info->fork_semaphore);
	sem_post(info->protectors[number]);
}

void	*philosopher(void *value)
{
	t_briefcase *info;

	info = value;
	while (*info->death_flag == -1)
	{
		eat(info);
		print_state("is sleeping", info->number + 1, info->print,
						info->death_flag);
		if (*info->death_flag != -1)
			break ;
		ft_sleep(info->time_to_sleep);
		print_state("is thinking", info->number + 1, info->print,
						info->death_flag);
	}
	return (NULL);
}
