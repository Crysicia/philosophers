#include "header.h"

void destroy_philosophers(t_philosopher *philosophers, int number_of_philosophers)
{
	int index;

	index = 0;
	while (index < number_of_philosophers)
	{
		// pthread_detach(philosophers[index].thread);
		index++;
	}
	free(philosophers);
}

bool init_single_philosopher(t_philosopher *philosopher, int index, pthread_mutex_t *left_fork, pthread_mutex_t *right_fork, unsigned long current_time)
{
	// if (pthread_create(&philosopher->thread))
	// 	return (false);
	philosopher->index			= index;
	philosopher->left_fork		= left_fork;
	philosopher->right_fork		= right_fork;
	philosopher->state			= THINKING;
	philosopher->last_meal		= 0;
	philosopher->last_sleep		= 0;
	philosopher->starting_time	= current_time;
	return (true);
}

t_philosopher *init_philosophers(int number_of_philosophers, pthread_mutex_t *forks)
{
	int index;
	t_philosopher *philosophers;
	unsigned long current_time;

	index = 0;
	current_time = get_current_time();
	philosophers = malloc(sizeof(t_philosopher) * number_of_philosophers);
	if (!philosophers)
		return (NULL);
	while (index < number_of_philosophers)
	{
		if (!init_single_philosopher(&philosophers[index], index, &forks[index], &forks[(index + 1) % number_of_philosophers], current_time))
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
	(void) forks;
	(void) number_of_forks;
	return ;
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
