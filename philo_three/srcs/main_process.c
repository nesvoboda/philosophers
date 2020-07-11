/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_process.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/29 12:58:17 by ashishae          #+#    #+#             */
/*   Updated: 2020/07/11 18:29:20 by ashishae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

void	kill_all_processes(int total, pid_t *processes)
{
	int i;

	i = 0;
	while (i < total)
	{
		kill(processes[i], SIGTERM);
		i++;
	}
}

void	wait_children(t_briefcase *proto)
{
	int	i;
	int	status;
	int	temp;

	i = 0;
	status = 42;
	temp = -10;
	while (i < proto->total)
	{
		status = 42;
		waitpid(-1, &status, 0);
		if ((WIFEXITED(status) || WIFSIGNALED(status)))
		{
			temp = WEXITSTATUS(status);
			// printf("Got exit %d\n", temp);
			if (temp == 0)
				return ;
			else if (temp == 1)
				i += 1;
		}
	}
	return ;
}
