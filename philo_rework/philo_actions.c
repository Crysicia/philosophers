#include "header.h"

void display_state(t_philosopher *philosopher, t_state state)
{
	static char *messages[5] = { "is thinking", "has taken a fork", "is eating", "is sleeping", "died" };

	pthread_mutex_lock(philosopher->write_lock);
	if (is_simulation_running(philosopher->simulation)
		&& (state == philo_get_state(philosopher) || state == FORK))
		printf("%lu %d %s\n", get_elapsed_time(philosopher), philosopher->index + 1, messages[state]);
	pthread_mutex_unlock(philosopher->write_lock);
}

void philo_take_fork(t_philosopher *philosopher, pthread_mutex_t *fork)
{
	pthread_mutex_lock(fork);
	display_state(philosopher, FORK);
}

void philo_eat(t_philosopher *philosopher)
{
	philo_take_fork(philosopher, philosopher->right_fork);
	philo_take_fork(philosopher, philosopher->left_fork);
	philo_set_duration(philosopher, &philosopher->last_meal, get_current_time());
	philo_set_state(philosopher, EATING);

	display_state(philosopher, EATING);
	ft_msleep(philosopher->time_to_eat - 1);
	pthread_mutex_unlock(philosopher->right_fork);
	pthread_mutex_unlock(philosopher->left_fork);
	philosopher->number_of_meals++;
	if (philosopher->number_of_meals == philosopher->max_meals)
		increment_total_meals(philosopher->simulation);
}

void philo_sleep(t_philosopher *philosopher)
{
	philo_set_duration(philosopher, &philosopher->last_sleep, get_current_time());
	philo_set_state(philosopher, SLEEPING);
	display_state(philosopher, SLEEPING);
	ft_msleep(philosopher->time_to_sleep - 1);
}

void philo_think(t_philosopher *philosopher)
{
	philo_set_state(philosopher, THINKING);
	display_state(philosopher, THINKING);
}
