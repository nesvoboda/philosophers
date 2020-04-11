/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/07 18:02:25 by ashishae          #+#    #+#             */
/*   Updated: 2020/04/10 13:03:06 by ashishae         ###   ########.fr       */
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
** Each philosopher will get his (or her) briefcase containing their
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
int g_time_to_eat;
int g_total;
int g_lastmeal;
pthread_mutex_t *g_forks;
pthread_mutex_t g_print;
