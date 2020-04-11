/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/06 17:04:19 by ashishae          #+#    #+#             */
/*   Updated: 2020/04/11 16:04:56 by ashishae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	ft_atoi(const char *str)
{
	int		nbr;
	int		sign;

	nbr = 0;
	sign = 1;
	while ((*str) == '\t' || (*str) == '\n' || (*str) == '\v' || (*str) == '\f'
			|| (*str) == '\r' || (*str) == ' ')
		str++;
	if ((*str) == '-' || (*str) == '+')
	{
		sign *= ((*str) == '-' ? -1 : 1);
		str++;
	}
	while ((*str) != '\0' && (*str) >= '0' && (*str) <= '9')
	{
		nbr *= 10;
		nbr += (*str) - '0';
		str++;
	}
	return (nbr * sign);
}

int		ft_strlen(char *str)
{
	int i;
	i = 0;
	while (str[i])
		i++;
	return (i);
}

void	ft_putchar(char c)
{
	write(1, &c, 1);
}

void	ft_putstr(char *str)
{
	write(1, str, ft_strlen(str));
}


// TODO: adapt for long
void	ft_putnbr(long n)
{
	long long int nbr;

	if (n < 0)
	{
		ft_putchar('-');
		nbr = (n == -2147483648) ? 2147483648 : -n;
	}
	else
		nbr = n;
	if (nbr > 9)
	{
		ft_putnbr(nbr / 10);
		ft_putnbr(nbr % 10);
	}
	if (nbr <= 9)
		ft_putchar(nbr + '0');
}

long get_time(void)
{
	struct timeval tp;
	gettimeofday(&tp, NULL);
	long seconds = tp.tv_sec * 1000;
	seconds += tp.tv_usec / 1000;
	return (seconds);
}

void print_state(char *state, int number, pthread_mutex_t *print_mutex)
{
	// printf("Print_state got mutex %p\n", print_mutex);
	long time;

	time = get_time();
	pthread_mutex_lock(print_mutex);
	ft_putnbr(time);
	ft_putchar(' ');
	ft_putnbr(number);
	ft_putchar(' ');
	ft_putstr(state);
	ft_putchar('\n');
	pthread_mutex_unlock(print_mutex);
}

void print_state2(char *state, int number, pthread_mutex_t *print_mutex, long time)
{
	// printf("Print_state got mutex %p\n", print_mutex);
	pthread_mutex_lock(print_mutex);
	ft_putnbr(time);
	ft_putchar(' ');
	ft_putnbr(number);
	ft_putchar(' ');
	ft_putstr(state);
	ft_putchar('\n');
	pthread_mutex_unlock(print_mutex);
}

int left_fork(int total, int number)
{
	if (number == 0)
		return (total - 1);
	else
		return (number - 1);
}

int right_fork(int total, int number)
{
		return (number);
}

void eat(t_briefcase *info)
{
	int left;
	int right;
	int number;

	number = info->number;
	left = left_fork(info->total, number);
	right = right_fork(info->total, number);
	pthread_mutex_lock(info->forks[left]);
	pthread_mutex_lock(info->forks[right]);
	// pthread_mutex_lock(info->protectors[number]);
	print_state("is eating", number, info->print);
	info->meal_counts[number] += 1;
	info->lastmeal[number] = get_time();
	// printf("Meal counts is at %p for %d\n", info->meal_counts, number);
	usleep(info->time_to_eat * 1000);
	// pthread_mutex_unlock(info->protectors[number]);
	pthread_mutex_unlock(info->forks[left]);
	pthread_mutex_unlock(info->forks[right]);
}

void *philosopher(void *value)
{
	t_briefcase *info;
	
	info = value;
	printf("%d started at %ld\n", info->number, get_time());
	// printf("#%d got forks 0 %p\n", info->number, info->forks[2]);
	while (1)
	{
		eat(info);
		print_state("is sleeping", info->number, info->print);
		usleep(info->time_to_sleep * 1000);
		print_state("is thinking", info->number, info->print);
	}
	return (NULL);
}

t_briefcase *give_briefcase(int number, t_briefcase proto)
{
	t_briefcase *new_briefcase;
	
	new_briefcase = malloc(sizeof(t_briefcase));
	new_briefcase->time_to_sleep = proto.time_to_sleep;
	new_briefcase->time_to_eat = proto.time_to_eat;
	new_briefcase->time_to_die = proto.time_to_die;
	new_briefcase->total = proto.total;
	new_briefcase->forks = proto.forks;
	new_briefcase->lastmeal = proto.lastmeal;
	new_briefcase->number = number;
	new_briefcase->print = proto.print;
	new_briefcase->death_flag = proto.death_flag;
	new_briefcase->meal_counts = proto.meal_counts;
	new_briefcase->protectors = proto.protectors;
	return new_briefcase;
}

void	init_mutexes(t_briefcase *proto)
{
	pthread_mutex_t	*forks;
	pthread_mutex_t	*protectors;
	pthread_mutex_t	*mtx;
	int				i;

	i = 0;
	mtx = malloc(sizeof(pthread_mutex_t));
	pthread_mutex_init(mtx, NULL);
	proto->print = mtx;
	forks = malloc(sizeof(pthread_mutex_t) * (proto->total));
	protectors = malloc(sizeof(pthread_mutex_t) * (proto->total));
	proto->forks = malloc(sizeof(pthread_mutex_t*) * (proto->total));
	proto->protectors = malloc(sizeof(pthread_mutex_t*) * (proto->total));
	while (i < proto->total)
	{
		pthread_mutex_init(&forks[i], NULL);
		pthread_mutex_init(&protectors[i], NULL);
		proto->forks[i] = &forks[i];
		proto->protectors[i] = &protectors[i];
		i++;
	}
}

void	init_arrays(t_briefcase *proto)
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
	proto->meal_counts = meal_counts;
}

/*
** check_dead goes through all last_meal timestamps and checks if a philosopher
** hasn't had a meal in more than time_to_die miliseconds.
*/

int		check_dead(t_briefcase proto)
{
	int		i;
	long	now;

	now = get_time();
	// printf("Last meals is:\n");
	// for (int y = 0; y < proto.total; y++)
	// 	printf("i: %d, lastmeal: %ld, difference with now: %ld\n", y, proto.lastmeal[y], now - proto.lastmeal[y]);
	i = 0;
	while (i < proto.total)
	{
		if ((int)(now - proto.lastmeal[i]) >= proto.time_to_die)
			return (i);
		i++;
	}
	return (0);

}

t_monitor *give_monitor(int i, t_briefcase proto)
{
	t_monitor *result;

	result = malloc(sizeof(t_monitor));
	result->protectors = proto.protectors;
	result->lastmeal = proto.lastmeal;
	result->number = i;
	result->time_to_die = proto.time_to_die;
	result->death_flag = proto.death_flag;
	result->print = proto.print;
	result->meal_counts = proto.meal_counts;
	result->eat_target = proto.eat_target;
	return (result);
}

void	*monitoring_thread(void *value)
{
	t_briefcase *monitor;
	long	now;
	long 	diff;

	monitor = value;
	now = get_time();
	while (1)
	{

		now = get_time();
		// pthread_mutex_lock(monitor->protectors[monitor->number]);
		diff = now - monitor->lastmeal[monitor->number];
		// printf("Diff: %ld\n", diff);
		// pthread_mutex_unlock(monitor->protectors[monitor->number]);

		if (diff >= monitor->time_to_die)
		{
			if (*(monitor->death_flag) == 0)
			{
				print_state("died", monitor->number, monitor->print);
				pthread_mutex_lock(monitor->print);
				*(monitor->death_flag) = monitor->number;
			}
			return (NULL);
		}
		usleep(8);
	}
	return (NULL);
}

void init_threads(t_briefcase proto)
{
	pthread_t	*thread_group;
	pthread_t	*monitoring_threads;
	t_briefcase **briefcases;
	int			i;

	i = 0;
	briefcases = malloc(sizeof(t_briefcase *) * proto.total);
	for (int j = 0; j < proto.total; j++)
	{
		briefcases[j] = give_briefcase(j, proto);
	}
	thread_group = malloc(sizeof(pthread_t) * proto.total);
	monitoring_threads = malloc(sizeof(pthread_t) * proto.total);
	while (i < proto.total)
	{
		pthread_create(&thread_group[i], NULL, philosopher, briefcases[i]);
		usleep(5);
		i++;
	}
	// i = 0;
	// while (i < proto.total)
	// {
	// 	pthread_create(&monitoring_threads[i], NULL, monitoring_thread, briefcases[i]);
	// 	i++;
	// }
}

int		check_exit_conditions(t_briefcase proto, int death_flag)
{
	int i;

	i = 0;
	if (death_flag != 0)
		return (1);
	if (proto.eat_target > 0)
	{
		while (i < proto.total)
		{
			if (proto.meal_counts[i] < proto.eat_target)
				break;
			i++;
		}
		if (i == proto.total)
			return (1);
	}
	return (0);
}

int threading (t_briefcase proto)
{
	int			dead;
	int			death_flag;

	death_flag = 0;
	proto.death_flag = &death_flag;
	// printf("Parameters: %d %d %d %d\n", proto.total, proto.time_to_die, proto.time_to_eat, proto.time_to_sleep);
	init_mutexes(&proto);
	init_arrays(&proto);
	init_threads(proto);
	
	while (1)
	{
		if (check_exit_conditions(proto, death_flag))
			return (0);
		usleep(5);

	}
	return 0;
}

int	set_parameters(int ac, char **av, t_briefcase *proto)
{
	if (ac < 5)
		return (-1);
	proto->total = ft_atoi(av[1]);
	proto->time_to_die = ft_atoi(av[2]);
	proto->time_to_eat = ft_atoi(av[3]);
	proto->time_to_sleep = ft_atoi(av[4]);
	if (ac > 5)
		proto->eat_target = ft_atoi(av[5]);
	else
		proto->eat_target = -1;
	return (0);
}

int main(int ac, char **av)
{
	t_briefcase proto;

	if (set_parameters(ac, av, &proto) < 0)
		return (0);
	return (threading(proto));
}