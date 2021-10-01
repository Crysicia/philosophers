#include "header.h"

void destroy_philosophers(t_philosopher *philosophers, int number_of_philosophers)
{
	int index;

	index = 0;
	while (index < number_of_philosophers)
	{
		destroy_lock(philosophers[index].access_lock);
		index++;
	}
	free(philosophers);
}

bool init_single_philosopher(t_philosopher *philosopher, int index, t_simulation *simulation, pthread_mutex_t *forks)
{
	philosopher->access_lock	= init_lock();
	philosopher->simulation		= simulation;
	philosopher->index			= index;
	philosopher->left_fork		= &forks[index];
	philosopher->right_fork		= &forks[(index + 1) % simulation->number_of_philosophers];
	philosopher->number_of_meals = 0;
	philosopher->last_meal		= 0;
	philosopher->last_sleep		= 0;
	if (!philosopher->access_lock)
		return (false);
	return (true);
}

t_philosopher *init_philosophers(t_simulation *simulation, pthread_mutex_t *forks)
{
	int index;
	int number_of_philosophers;
	t_philosopher *philosophers;

	index = 0;
	number_of_philosophers = simulation->number_of_philosophers;
	philosophers = malloc(sizeof(t_philosopher) * number_of_philosophers);
	if (!philosophers)
		return (NULL);
	while (index < number_of_philosophers)
	{
		if (!init_single_philosopher(&philosophers[index], index, simulation, forks))
		{
			free(philosophers);
			return (NULL);
		}
		index++;
	}
	return (philosophers);
}

void destroy_forks(pthread_mutex_t *forks, int number_of_forks)
{
	int index;

	index = 0;
	while (index < number_of_forks)
	{
		pthread_mutex_destroy(&forks[index]);
		index++;
	}
	free(forks);
}

pthread_mutex_t *init_forks(int number_of_forks)
{
	pthread_mutex_t *forks;
	int index;

	index = 0;
	forks = malloc(sizeof(pthread_mutex_t) * number_of_forks);
	if (!forks)
		return (NULL);
	while (index < number_of_forks)
	{
		if (pthread_mutex_init(&forks[index], NULL) != 0)
		{
			destroy_forks(forks, index);
			return (NULL);
		}
		index++;
	}
	return (forks);
}
