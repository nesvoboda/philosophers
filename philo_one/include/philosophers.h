/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/07 18:02:25 by ashishae          #+#    #+#             */
/*   Updated: 2020/06/07 16:46:55 by ashishae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>

typedef struct s_conditions
{
	int time_to_sleep;
	int time_to_eat;
	int total;	/* total philosophers */
}				t_conditions;

/*
** Each philosopher will get his or her briefcase containing their
** number, address to all mutexes and other important stuff.
*/

typedef struct	s_briefcase
{
	int				number;
	int				time_to_eat;
	int				time_to_sleep;
	int				time_to_die;
	int				eat_target;
	int				total;
	long			*lastmeal; /* to write when they had meals */
	int				*meal_counts; /* to write when they had meals */
	pthread_mutex_t	**forks; /* mutexes for forks */
	pthread_mutex_t	**protectors; /* mutexes for forks */
	pthread_mutex_t	*print;
	int				*death_flag;
}				t_briefcase;

typedef struct	s_monitor
{
	int				number;
	long			*lastmeal;
	int				time_to_die;
	pthread_mutex_t	**protectors;
	int				*death_flag;
	int				*meal_counts; /* to write when they had meals */
	pthread_mutex_t	*print;
	int				eat_target;

}				t_monitor;

int g_time_to_eat;
int g_time_to_sleep;
int g_total;
int g_lastmeal;
pthread_mutex_t *g_forks;
pthread_mutex_t g_print;


/*
** libft_lite.c — small libc-like functions
*/

int			ft_atoi(const char *str);
int			ft_strlen(char *str);
void		ft_putchar(char c);
void		ft_putstr(char *str);
void		ft_putnbr(long n);

/*
** structure_management.c — fuctions that allocate memory for structures
** and set default values
*/

t_monitor	*give_monitor(int i, t_briefcase proto);
void		init_arrays(t_briefcase *proto);
t_briefcase	*give_briefcase(int number, t_briefcase proto);
void		init_mutexes(t_briefcase *proto);

/*
** utilities.c — fuctions that do practical things like get time,
** concurrently print statuses and so on.
*/

long		get_time(void);
void		print_state(char *state, int number,
							pthread_mutex_t *print_mutex, int *death_flag);
int			left_fork(int total, int number);
int			right_fork(int total, int number);

/*
** philosopher.c — fuctions that implement the functionality of a philosopher.
*/

void		eat(t_briefcase *info);
void		*philosopher(void *value);

/*
** monitoring.c — fuctions that implement the monitoring threads. They
** check if a philosopher has 'died'.
*/

int			check_dead(t_briefcase proto);
void		*monitoring_thread(void *value);


/*
** threading.c — fuctions that start and control threads.
*/

void		init_threads(t_briefcase proto);
int			check_exit_conditions(t_briefcase proto, int death_flag);
int			threading (t_briefcase proto);