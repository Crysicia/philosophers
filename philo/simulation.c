/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpassera <lpassera@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/01 15:56:27 by lpassera          #+#    #+#             */
/*   Updated: 2021/10/18 14:27:10 by lpassera         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

void	*routine(void *arg)
{
	t_philosopher	*philosopher;

	philosopher = arg;
	if (philosopher->simulation->number_of_philosophers == 1)
	{
		philo_take_fork(philosopher, philosopher->right_fork);
		ft_msleep(philosopher->simulation->time_to_die);
		pthread_mutex_unlock(philosopher->right_fork);
		return (NULL);
	}
	if (philosopher->index % 2)
		ft_msleep(philosopher->simulation->time_to_sleep);
	while (is_simulation_running(philosopher->simulation))
	{
		philo_think(philosopher);
		philo_eat(philosopher);
		philo_sleep(philosopher);
	}
	return (NULL);
}

bool	launch_threads(t_philosopher *philosophers, int number_of_philosophers)
{
	int	index;

	index = 0;
	while (index < number_of_philosophers)
	{
		if (pthread_create(&philosophers[index].thread, NULL,
				routine, &philosophers[index]))
			return (false);
		index++;
	}
	return (true);
}

void	wait_threads(t_simulation *simulation)
{
	int	index;

	index = 0;
	while (index < simulation->number_of_philosophers)
	{
		pthread_join(simulation->philosophers[index].thread, NULL);
		pthread_detach(simulation->philosophers[index].thread);
		index++;
	}
	pthread_join(simulation->watcher, NULL);
}

bool	launch_watcher(t_simulation *simulation)
{
	if (pthread_create(&simulation->watcher, NULL, watcher, simulation))
		return (false);
	return (true);
}

unsigned int	launch_simulation(t_simulation *simulation)
{
	simulation->forks = init_forks(simulation->number_of_philosophers);
	if (!simulation->forks)
		return (ERR_COULD_NOT_INITIALIZE_FORKS);
	simulation->philosophers = init_philosophers(simulation, simulation->forks);
	if (!simulation->philosophers)
	{
		destroy_forks(simulation->forks, simulation->number_of_philosophers);
		return (ERR_COULD_NOT_INITIALIZE_PHILOS);
	}
	if (!launch_threads(simulation->philosophers,
			simulation->number_of_philosophers))
		return (ERR_COULD_NOT_CREATE_THREAD);
	launch_watcher(simulation);
	wait_threads(simulation);
	return (SUCCESS);
}
