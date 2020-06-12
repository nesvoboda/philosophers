/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilities.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/07 14:14:20 by ashishae          #+#    #+#             */
/*   Updated: 2020/06/12 18:47:17 by ashishae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

long	get_time(void)
{
	struct timeval	tp;
	long			seconds;

	gettimeofday(&tp, NULL);
	seconds = tp.tv_sec * 1000;
	seconds += tp.tv_usec / 1000;
	return (seconds);
}

void	print_state(char *state, int number, pthread_mutex_t *print_mutex,
					int *death_flag)
{
	long	timev;

	if (*death_flag != -1)
		return ;
	timev = get_time();
	pthread_mutex_lock(print_mutex);
	ft_putnbr(timev);
	ft_putchar(' ');
	ft_putnbr(number);
	ft_putchar(' ');
	ft_putstr(state);
	ft_putchar('\n');
	pthread_mutex_unlock(print_mutex);
}

int		left_fork(int total, int number)
{
	if (number == 0)
		return (total - 1);
	else
		return (number - 1);
}

int		right_fork(int total, int number)
{
	(void)total;
	return (number);
}
