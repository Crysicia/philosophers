#include "header.h"

void display_state(t_philosopher *philosopher, t_state state)
{
	static char *messages[5] = { "is thinking", "has taken a fork", "is eating", "is sleeping", "died" };

	if (state == philosopher->state || state == FORK)
		printf("%lu %d %s\n", get_elapsed_time(philosopher), philosopher->index + 1, messages[state]);
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
	philosopher->last_meal	= get_current_time();
	philosopher->state		= EATING;
	display_state(philosopher, EATING);
	ft_msleep(philosopher->simulation->time_to_eat - 1);
	pthread_mutex_unlock(philosopher->right_fork);
	pthread_mutex_unlock(philosopher->left_fork);
}

void philo_sleep(t_philosopher *philosopher)
{
	philosopher->last_sleep = get_current_time();
	philosopher->state = SLEEPING;
	display_state(philosopher, SLEEPING);
	ft_msleep(philosopher->simulation->time_to_sleep - 1);
}

void philo_think(t_philosopher *philosopher)
{
	philosopher->state = THINKING;
	display_state(philosopher, THINKING);
}
