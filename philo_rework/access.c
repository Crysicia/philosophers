#include "header.h"

t_state philo_get_state(t_philosopher *philosopher)
{
	t_state state;

	pthread_mutex_lock(philosopher->access_lock);
	state = philosopher->state;
	pthread_mutex_unlock(philosopher->access_lock);
	return (state);
}

void philo_set_state(t_philosopher *philosopher, t_state state)
{
	pthread_mutex_lock(philosopher->access_lock);
	philosopher->state = state;
	pthread_mutex_unlock(philosopher->access_lock);
}

unsigned long philo_get_duration(t_philosopher *philosopher, unsigned long *philo_duration)
{
	unsigned long duration;

	pthread_mutex_lock(philosopher->access_lock);
	duration = *philo_duration;
	pthread_mutex_unlock(philosopher->access_lock);
	return (duration);
}

void philo_set_duration(t_philosopher *philosopher, unsigned long *philo_duration, unsigned long set_duration)
{
	pthread_mutex_lock(philosopher->access_lock);
	*philo_duration = set_duration;
	pthread_mutex_unlock(philosopher->access_lock);
}