/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/07 14:23:59 by ashishae          #+#    #+#             */
/*   Updated: 2020/07/24 15:04:22 by ashishae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

void	eat(t_briefcase *info)
{
	int number;

	number = info->number;
	sem_wait(info->grab_semaphore);
	sem_wait(info->fork_semaphore);
	print_state("has taken a fork", number + 1, info->print);
	sem_wait(info->fork_semaphore);
	print_state("has taken a fork", number + 1, info->print);
	sem_post(info->grab_semaphore);
	sem_wait(info->protectors[number]);
	info->last_meal = get_time();
	print_state("is eating", number + 1, info->print);
	ft_sleep(info->time_to_eat);
	sem_post(info->protectors[number]);
	sem_post(info->fork_semaphore);
	sem_post(info->fork_semaphore);
}

void	*philosopher(void *value)
{
	t_briefcase	*info;
	int			i;

	info = value;
	i = 0;
	while (1)
	{
		eat(info);
		i += 1;
		print_state("is sleeping", info->number + 1, info->print);
		if (info->eat_target > 0 && i >= info->eat_target)
		{
			if (info->death_flag != 0)
				exit(1);
			else
				exit(0);
		}
		ft_sleep(info->time_to_sleep);
		print_state("is thinking", info->number + 1, info->print);
	}
	return (NULL);
}
