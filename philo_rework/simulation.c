#include "header.h"

void destroy_philosophers(t_philosopher *philosophers, int number_of_philosophers)
{
	int index;

	index = 0;
	while (index < number_of_philosophers)
	{
		pthread_detach(philosophers[index].thread);
		index++;
	}
	free(philosophers);
}

bool init_single_philosopher(t_philosopher *philosopher, int index, pthread_mutex_t *left_fork, pthread_mutex_t *right)
{
	// if (pthread_create(&philosopher->thread))
	// 	return (false);
	philosopher->index = index;
	philosopher->left_fork = left_fork;
	philosopher->right_fork = right_fork;
}

t_philosopher *init_philosophers(int number_of_philosophers)
{
	int index;
	t_philosopher *philosophers;

	index = 0;
	philosophers = malloc(sizeof(t_philosopher) * number_of_philosophers);
	if (!philosophers)
		return (NULL);
	while (index < number_of_philosophers)
	{
		if (!init_single_philosopher(&philosophers[index], index))
		{
			free(philosophers);
			return (NULL);
		}
		index++;
	}
	return (philosophers);
}

unsigned int launch_simulation(t_simulation *simulation)
{
	t_philosopher *philosophers;

	philosophers = init_philosophers(simulation->number_of_philosophers);
	if (!philosophers)
		return (ERR_COULD_NOT_INITIALIZE_PHILOS);
	launch_threads(philosophers);
}
