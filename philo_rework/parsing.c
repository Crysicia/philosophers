/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpassera <lpassera@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/01 14:57:46 by lpassera          #+#    #+#             */
/*   Updated: 2021/10/01 16:11:28 by lpassera         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

pthread_mutex_t	*init_lock(void)
{
	pthread_mutex_t	*lock;

	lock = malloc(sizeof(pthread_mutex_t));
	if (!lock || pthread_mutex_init(lock, NULL) != 0)
	{
		free(lock);
		return (NULL);
	}
	return (lock);
}

void	destroy_lock(pthread_mutex_t *lock)
{
	if (!lock)
		return ;
	pthread_mutex_destroy(lock);
	free(lock);
}

int	ft_atoi_strict(const char *str)
{
	int	result;

	result = 0;
	while (*str)
	{
		if (*str < '0' || *str > '9')
			return (-1);
		result = (result * 10) + (*str - '0');
		if (result < 0)
			return (-2);
		str++;
	}
	return (result);
}

bool	is_simulation_valid(t_simulation *simulation, int argc)
{
	return (!simulation->write_lock
		|| !simulation->access_lock
		|| simulation->number_of_philosophers < 0
		|| simulation->time_to_die < 0
		|| simulation->time_to_eat < 0
		|| simulation->time_to_sleep < 0
		|| (simulation->number_of_meals < 0 && argc == 6));
}

t_simulation	*parse_arguments(int argc, char *argv[])
{
	t_simulation	*simulation;

	if (argc != 5 && argc != 6)
		return (NULL);
	simulation = malloc(sizeof(t_simulation));
	if (!simulation)
		return (NULL);
	simulation->is_running = true;
	simulation->write_lock = init_lock();
	simulation->access_lock = init_lock();
	simulation->number_of_philosophers = ft_atoi_strict(argv[NB_OF_PHILO]);
	simulation->time_to_die = ft_atoi_strict(argv[TIME_TO_DIE]);
	simulation->time_to_eat = ft_atoi_strict(argv[TIME_TO_EAT]);
	simulation->time_to_sleep = ft_atoi_strict(argv[TIME_TO_SLEEP]);
	simulation->meals_ate = 0;
	simulation->number_of_meals = -1;
	if (argc == 6)
		simulation->number_of_meals = ft_atoi_strict(argv[NUMBER_OF_MEALS]);
	simulation->starting_time = get_current_time();
	if (is_simulation_valid(simulation, argc))
	{
		destroy_simulation(simulation);
		return (NULL);
	}
	return (simulation);
}
