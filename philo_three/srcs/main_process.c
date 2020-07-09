/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_process.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/29 12:58:17 by ashishae          #+#    #+#             */
/*   Updated: 2020/07/09 16:30:14 by ashishae         ###   ########.fr       */
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
	int i;
	int status;
	int fin;

	i = 0;
	status = 42;
	fin = 0;
	while (i < proto->total)
	{
		waitpid(-1, &status, 0);
		if (WIFEXITED(status) && WEXITSTATUS(status) == 1)
			i += 1;
		else if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
			return ;
	}
	return ;
}
