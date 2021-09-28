#include "header.h"

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
