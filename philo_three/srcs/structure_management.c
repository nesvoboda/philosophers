/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structure_management.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/07 14:10:45 by ashishae          #+#    #+#             */
/*   Updated: 2020/06/26 18:51:22 by ashishae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"
#include <errno.h>

/*
** init_arrays resets all arrays in the 'briefcase' prototype called proto.
*/

void		init_arrays(t_briefcase *proto)
{
	long	*last_meal;
	int		*meal_counts;
	int		i;

	last_meal = malloc(sizeof(long) * proto->total);
	meal_counts = malloc(sizeof(int) * proto->total);
	last_meal[0] = get_time();
	i = 0;
	while (i < proto->total)
	{
		last_meal[i] = last_meal[0];
		meal_counts[i] = 0;
		i++;
	}
	proto->lastmeal = last_meal;
	proto->last_meal = last_meal[0];
	proto->meal_count = 0;
	proto->meal_counts = meal_counts;
	proto->processes = malloc(sizeof(pid_t) * proto->total);
}

/*
** give_briefcase creates a new 'briefcase' from the prototype proto.
*/

t_briefcase	*give_briefcase(int number, t_briefcase *proto)
{
	t_briefcase *new_briefcase;

	new_briefcase = malloc(sizeof(t_briefcase));
	new_briefcase->time_to_sleep = proto->time_to_sleep;
	new_briefcase->time_to_eat = proto->time_to_eat;
	new_briefcase->time_to_die = proto->time_to_die;
	new_briefcase->total = proto->total;
	new_briefcase->lastmeal = proto->lastmeal;
	new_briefcase->number = number;
	new_briefcase->print = proto->print;
	new_briefcase->death_flag = proto->death_flag;
	new_briefcase->meal_counts = proto->meal_counts;
	new_briefcase->protectors = proto->protectors;
	new_briefcase->fork_semaphore = proto->fork_semaphore;
	new_briefcase->processes = proto->processes;
	new_briefcase->last_meal = proto->last_meal;
	new_briefcase->meal_count = proto->meal_count;
	new_briefcase->eat_target = proto->eat_target;
	return (new_briefcase);
}

/*
** new_semaphore tries to create a new semaphore with name name. If it
** already exists, it will be unlinked and recreated to ensure compliance
** with problem conditions.
*/

sem_t		*new_semaphore(char *name, int value)
{
	sem_t	*result;
	int		errno;

	result = sem_open(name, O_CREAT | O_EXCL, S_IRWXU, value);
	if (result == SEM_FAILED && errno == EEXIST)
	{
		sem_unlink(name);
		return (sem_open(name, O_CREAT | O_EXCL, S_IRWXU, value));
	}
	else
		return (result);
}

/*
** init_mutexes creates mutexes in proto (the prototype of all future)
** 'briefcases'.
** Peculiar moment: the variable 'name' is a 12-character array
** because the number of philosophers is an int. Therefore, a
** semaphore name p[n], where n is the number of philosophers,
** can occupy no more than 12 characters (one for 'p', 10 for n, one for '\0').
*/

int			init_semaphores(t_briefcase *proto)
{
	int		i;
	char	name[12];
	sem_t	*sem;

	i = 0;
	proto->print = new_semaphore("print", 1);
	proto->death_flag = new_semaphore("death_flag ", 1);
	proto->fork_semaphore = new_semaphore("forks", proto->total);
	proto->protectors = malloc(sizeof(sem_t *) * (proto->total));
	name[0] = 'p';
	while (i < proto->total)
	{
		print_long((unsigned long)i, &name[1]);
		sem = new_semaphore(name, 1);
		if (sem != SEM_FAILED)
			proto->protectors[i] = sem;
		else
			return (-1);
		i++;
	}
	return (0);
}
