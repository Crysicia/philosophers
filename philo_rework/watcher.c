#include "header.h"

bool is_simulation_running(t_simulation *simulation)
{
	bool ret;

	pthread_mutex_lock(simulation->access_lock);
	ret = simulation->is_running;
	pthread_mutex_unlock(simulation->access_lock);
	return (ret);
}

bool philo_is_dead(t_philosopher *philosopher, unsigned int time_to_die)
{
	bool ret;

	pthread_mutex_lock(philosopher->access_lock);
	if (philosopher->state == DEAD)
		ret = true;
	else if (!philosopher->last_meal)
		ret = (get_current_time() - philosopher->simulation->starting_time >= time_to_die);
	else
		ret = (get_current_time() - philosopher->last_meal >= time_to_die);
	pthread_mutex_unlock(philosopher->access_lock);
	return (ret);
}

void *watcher(void *arg)
{
	int index;
	t_simulation *simulation;
	t_philosopher *philosophers;

	simulation		= arg;
	philosophers	= simulation->philosophers;
	while (is_simulation_running(simulation))
	{
		index = 0;
		while (index < simulation->number_of_philosophers)
		{
			if (philo_is_dead(&philosophers[index], simulation->time_to_die))
			{
				philo_set_state(&philosophers[index], DEAD);
				display_state(&philosophers[index], DEAD);
				pthread_mutex_lock(simulation->access_lock);
				simulation->is_running = false;
				pthread_mutex_unlock(simulation->access_lock);
				return (NULL);
			}
			index++;
		}
	}
	return (NULL);
}