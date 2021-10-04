/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_actions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpassera <lpassera@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/01 15:54:22 by lpassera          #+#    #+#             */
/*   Updated: 2021/10/04 14:20:45 by lpassera         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

void	display_state(t_philosopher *philosopher, t_state state)
{
	static char	*messages[5] = {"is thinking", "has taken a fork",
	"is eating", "is sleeping", "died"};

	pthread_mutex_lock(philosopher->simulation->write_lock);
	if (is_simulation_running(philosopher->simulation))
		printf("%lu %d %s\n", get_elapsed_time(philosopher),
			philosopher->index + 1, messages[state]);
	pthread_mutex_unlock(philosopher->simulation->write_lock);
}

void	philo_take_fork(t_philosopher *philosopher, pthread_mutex_t *fork)
{
	pthread_mutex_lock(fork);
	display_state(philosopher, FORK);
}

void	philo_eat(t_philosopher *philosopher)
{
	philo_take_fork(philosopher, philosopher->right_fork);
	philo_take_fork(philosopher, philosopher->left_fork);
	philo_set_duration(philosopher, &philosopher->last_meal,
		get_current_time());
	display_state(philosopher, EATING);
	ft_msleep(philosopher->simulation->time_to_eat);
	pthread_mutex_unlock(philosopher->right_fork);
	pthread_mutex_unlock(philosopher->left_fork);
	philosopher->number_of_meals++;
	if (philosopher->number_of_meals
		== philosopher->simulation->number_of_meals)
		increment_total_meals(philosopher->simulation);
}

void	philo_sleep(t_philosopher *philosopher)
{
	philo_set_duration(philosopher, &philosopher->last_sleep,
		get_current_time());
	display_state(philosopher, SLEEPING);
	ft_msleep(philosopher->simulation->time_to_sleep);
}

void	philo_think(t_philosopher *philosopher)
{
	display_state(philosopher, THINKING);
}
