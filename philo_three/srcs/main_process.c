/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_process.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/29 12:58:17 by ashishae          #+#    #+#             */
/*   Updated: 2020/06/30 17:46:33 by ashishae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

void	kill_all_processes(int total, pid_t *processes)
{
	int i;

	i = 0;
	while (i < total)
	{
		kill(processes[i], SIGKILL);
		i++;
	}
}

int		all_finished(int *array, int total)
{
	int i;

	i = 0;
	while (i < total)
	{
		if (array[i] != 1)
			return (0);
		i++;
	}
	return (1);
}

int		check_exits(int *finished, t_briefcase *proto)
{
	int i;
	int status;

	i = 0;
	status = 42;
	while (i < proto->total)
	{
		waitpid(proto->processes[i], &status, WNOHANG);
		if (WIFEXITED(status) && WEXITSTATUS(status) == 1)
			finished[i] = 1;
		else if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
			return (-1);
		i++;
	}
	return (0);
}

void	wait_children(t_briefcase *proto)
{
	int *finished;

	finished = malloc(sizeof(int) * proto->total);
	memset(finished, 0, proto->total);
	while (1)
	{
		if (check_exits(finished, proto) ||
			all_finished(finished, proto->total))
		{
			free(finished);
			return ;
		}
		usleep(50);
	}
}
