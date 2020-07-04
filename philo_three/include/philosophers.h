/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/07 18:02:25 by ashishae          #+#    #+#             */
/*   Updated: 2020/07/04 18:07:38 by ashishae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <stdio.h>
# include <pthread.h>
# include <unistd.h>
# include <stdlib.h>
# include <sys/time.h>
# include <string.h>
# include <semaphore.h>
# include <signal.h>
# include <sys/wait.h>
# include <errno.h>
# include <fcntl.h>

/*
** A 'briefcase' is a structure that contains:
** - conditions of the problem (i.e. time to eat, sleep or before death)
** - mutexes for forks, mutexes that protect philosophers from death
**   while they're eating, mutex for printing to stdout
** - addresses and variables to report when philosophers had their meals,
**   and how many meals they've had.
**
** Each philosopher will get his or her own 'briefcase'.
*/

typedef struct	s_briefcase
{
	int				number;
	int				time_to_eat;
	int				time_to_sleep;
	int				time_to_die;
	int				eat_target;
	int				total;
	sem_t			**protectors;
	sem_t			*print;
	sem_t			*fork_semaphore;
	pid_t			*processes;
	long			last_meal;
	int				meal_count;
}				t_briefcase;

/*
** This is a global variable that will store the UNIX timestamp of
** when the simulation started. It would be quite complex to pass it as
** a variable or store it in a structure, so it's a global.
** It is not supposed to be modified, it is correct for all threads at any
** moment of time.
** It allows us to print more friendly timestamps that start with 0.
*/

unsigned long	g_time_start;

/*
** libft_lite.c — small libc-like functions
*/

int				ft_atoi(const char *str);
int				ft_strlen(char *str);
void			ft_putchar(char c);
void			ft_putstr(char *str);
void			ft_putnbr(long n);

/*
** structure_management.c — fuctions that allocate memory for structures
** and set default values
*/

void			init_variables(t_briefcase *proto);
t_briefcase		*give_briefcase(int number, t_briefcase *proto);
int				init_semaphores(t_briefcase *proto);

/*
** utilities.c — fuctions that do practical things like get time,
** concurrently print statuses and so on.
*/

long			get_time(void);
void			print_state(char *state, int number, sem_t *print);
void			ft_puterr(char *str);

/*
** philosopher.c — fuctions that implement the functionality of a philosopher.
*/

void			eat(t_briefcase *info);
void			*philosopher(void *value);

/*
** monitoring.c — fuctions that implement the monitoring threads. They
** check if a philosopher has 'died'.
*/

void			*monitoring_thread(void *value);

/*
** threading.c — fuctions that start and control threads.
*/

void			*init_threads(t_briefcase *proto, t_briefcase **briefcases,
					pthread_t **monitors);
int				threading (t_briefcase proto, t_briefcase **briefcases);
void			kill_all_processes(int total, pid_t *processes);

/*
** print_long.c — a function that writes a long to a given character buffer.
** There's also ft_strlcpy which is used for printing out statuses.
*/

int				print_long(unsigned long n, char *result);
size_t			ft_strlcpy(char *dst, char *src, size_t dstsize);

/*
** main_process.c — the main process will wait on the children processes,
** and check if they have died (and terminate the simulation in this case)
*/

void			kill_all_processes(int total, pid_t *processes);
int				all_finished(int *array, int total);
int				check_exits(int *finished, t_briefcase *proto);
void			wait_children(t_briefcase *proto);

#endif
