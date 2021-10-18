/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   watcher.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpassera <lpassera@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/01 16:09:26 by lpassera          #+#    #+#             */
/*   Updated: 2021/10/18 14:30:22 by lpassera         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

bool	is_simulation_running(t_simulation *simulation)
{
	bool	ret;

	pthread_mutex_lock(simulation->access_lock);
	ret = simulation->is_running;
	pthread_mutex_unlock(simulation->access_lock);
	return (ret);
}

bool	philo_is_dead(t_philosopher *philosopher, unsigned int time_to_die)
{
	bool	ret;

	pthread_mutex_lock(philosopher->access_lock);
	if (!philosopher->last_meal)
		ret = (get_current_time() - philosopher->simulation->starting_time
				> time_to_die);
	else
		ret = (get_current_time() - philosopher->last_meal > time_to_die);
	pthread_mutex_unlock(philosopher->access_lock);
	return (ret);
}

void	unlock_all_forks(t_simulation *simulation)
{
	int	index;

	index = 0;
	while (index < simulation->number_of_philosophers)
	{
		pthread_mutex_unlock(&simulation->forks[index]);
		index++;
	}
}

void	*stop_simulation(t_simulation *simulation)
{
	pthread_mutex_lock(simulation->access_lock);
	simulation->is_running = false;
	pthread_mutex_unlock(simulation->access_lock);
	return (NULL);
}

void	*watcher(void *arg)
{
	int				index;
	t_simulation	*simulation;
	t_philosopher	*philosophers;

	simulation = arg;
	philosophers = simulation->philosophers;
	while (true)
	{
		index = 0;
		if (get_total_meals(simulation) >= simulation->number_of_philosophers)
			return (stop_simulation(simulation));
		while (index < simulation->number_of_philosophers)
		{
			if (philo_is_dead(&philosophers[index], simulation->time_to_die))
			{
				display_state(&philosophers[index], DEAD);
				return (stop_simulation(simulation));
			}
			index++;
		}
		usleep(500);
	}
	return (NULL);
}
