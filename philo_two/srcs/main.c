/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/06 17:04:19 by ashishae          #+#    #+#             */
/*   Updated: 2020/06/18 16:55:17 by ashishae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

int	set_parameters(int ac, char **av, t_briefcase *proto)
{
	if (ac < 5 || ac > 6)
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

int	main(int ac, char **av)
{
	t_briefcase proto;

	g_time_start = get_time();
	if (set_parameters(ac, av, &proto) < 0)
		return (0);
	return (threading(proto));
}
