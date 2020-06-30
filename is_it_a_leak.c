/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_it_a_leak.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/30 17:59:03 by ashishae          #+#    #+#             */
/*   Updated: 2020/06/30 17:59:33 by ashishae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>

int	main(void)
{
	pid_t pid;

	pid = fork();
	if (pid == 0)
	{
		usleep(500000000);
		printf("EXIT");
	}
	wait(NULL);
}
