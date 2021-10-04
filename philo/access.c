/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   access.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpassera <lpassera@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/01 16:09:25 by lpassera          #+#    #+#             */
/*   Updated: 2021/10/01 16:11:08 by lpassera         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

unsigned long	philo_get_duration(t_philosopher *philosopher,
	unsigned long *philo_duration)
{
	unsigned long	duration;

	pthread_mutex_lock(philosopher->access_lock);
	duration = *philo_duration;
	pthread_mutex_unlock(philosopher->access_lock);
	return (duration);
}

void	philo_set_duration(t_philosopher *philosopher,
	unsigned long *philo_duration, unsigned long set_duration)
{
	pthread_mutex_lock(philosopher->access_lock);
	*philo_duration = set_duration;
	pthread_mutex_unlock(philosopher->access_lock);
}

void	increment_total_meals(t_simulation *simulation)
{
	pthread_mutex_lock(simulation->access_lock);
	simulation->meals_ate++;
	pthread_mutex_unlock(simulation->access_lock);
}

int	get_total_meals(t_simulation *simulation)
{
	int	meals_ate;

	pthread_mutex_lock(simulation->access_lock);
	meals_ate = simulation->meals_ate;
	pthread_mutex_unlock(simulation->access_lock);
	return (meals_ate);
}
