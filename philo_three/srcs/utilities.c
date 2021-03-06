/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilities.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/07 14:14:20 by ashishae          #+#    #+#             */
/*   Updated: 2020/07/25 12:37:49 by ashishae         ###   ########.fr       */
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

void	print_state(char *state, int number, sem_t *print)
{
	long	timev;
	char	buffer[50];
	int		position;

	memset(buffer, 0, 50);
	timev = get_time() - g_time_start;
	position = print_long((unsigned long)timev, buffer);
	buffer[position] = ' ';
	position += print_long((unsigned long)number, &buffer[position + 1]) + 1;
	buffer[position] = ' ';
	position += ft_strlcpy(&buffer[position + 1],
		state, ft_strlen(state) + 1) + 1;
	buffer[position] = '\n';
	sem_wait(print);
	ft_putstr(buffer);
	sem_post(print);
}

void	rogue_print_state(char *state, int number)
{
	long	timev;
	char	buffer[50];
	int		position;

	memset(buffer, 0, 50);
	timev = get_time() - g_time_start;
	position = print_long((unsigned long)timev, buffer);
	buffer[position] = ' ';
	position += print_long((unsigned long)number, &buffer[position + 1]) + 1;
	buffer[position] = ' ';
	position += ft_strlcpy(&buffer[position + 1],
		state, ft_strlen(state) + 1) + 1;
	buffer[position] = '\n';
	ft_putstr(buffer);
}

void	ft_puterr(char *str)
{
	write(2, str, ft_strlen(str));
}

void	ft_sleep(int milis)
{
	long start_time;

	start_time = get_time();
	while (get_time() - start_time < milis)
	{
		usleep(FTSLEEP_DELAY);
	}
}
