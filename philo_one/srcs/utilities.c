/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilities.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/07 14:14:20 by ashishae          #+#    #+#             */
/*   Updated: 2020/07/04 14:31:26 by ashishae         ###   ########.fr       */
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

/*
** A typical status message is structured as follows:
** [timestamp (long)] [philosopher index (int)] [state (string)]\n
** Long is 19 digits long.
** Int is 9 digits long.
** The longest state is 16 characters long.
** We don't need more than 50 characters for printing out the status message.
*/

void	print_state(char *state, int number, pthread_mutex_t *print_mutex,
					int *death_flag)
{
	long	timev;
	char	buffer[50];
	int		position;

	if (*death_flag != -1)
		return ;
	memset(buffer, 0, 50);
	timev = get_time() - g_time_start;
	position = print_long((unsigned long)timev, buffer);
	buffer[position] = ' ';
	position += print_long((unsigned long)number, &buffer[position + 1]) + 1;
	buffer[position] = ' ';
	position += ft_strlcpy(&buffer[position + 1],
		state, ft_strlen(state) + 1) + 1;
	buffer[position] = '\n';
	pthread_mutex_lock(print_mutex);
	ft_putstr(buffer);
	pthread_mutex_unlock(print_mutex);
}

void	ft_puterr(char *str)
{
	write(2, str, ft_strlen(str));
}
