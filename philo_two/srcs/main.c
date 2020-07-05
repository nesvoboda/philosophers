/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/06 17:04:19 by ashishae          #+#    #+#             */
/*   Updated: 2020/07/05 16:20:25 by ashishae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

int check_argument(char *argument)
{
	int i;

	i = 0;
	while (argument[i])
	{
		if (!(argument[i] >= '0' && argument[i] <= '9') && ((i > 0) || 
			(i == 0 && argument[i] != '+' && argument[i] != '-')))
			return (-1);
		i++;
	}
	return (0);
}

int	set_parameters(int ac, char **av, t_briefcase *proto)
{
	int i;

	if (ac < 5 || ac > 6)
		return (-1);
	i = 1;
	while (i < ac)
		if (check_argument(av[i++]))
			return (-1);
	proto->total = ft_atoi(av[1]);
	proto->time_to_die = ft_atoi(av[2]);
	proto->time_to_eat = ft_atoi(av[3]);
	proto->time_to_sleep = ft_atoi(av[4]);
	if (ac == 6)
	{
		proto->eat_target = ft_atoi(av[5]);
		if (proto->eat_target < 0)
			return (-1);
	}
	else
		proto->eat_target = -1;
	if (proto->total <= 0 || proto->time_to_die < 0 || proto->time_to_eat < 0 ||
		proto->time_to_sleep < 0)
		return (-1);
	return (0);
}

int	main(int ac, char **av)
{
	t_briefcase proto;

	g_time_start = get_time();
	if (set_parameters(ac, av, &proto) < 0)
	{
		ft_puterr("Usage: ./philo_one [number of philosophers] [time to die]"
			" [time to eat] [time to sleep] ([meals before stop]).\n"
			"All arguments must be integers.\n");
		return (-1);
	}
	if (threading(proto) == -1)
		ft_putstr("Error while creating semaphores. "
					"Probably too many philosophers\n");
	return (0);
}
