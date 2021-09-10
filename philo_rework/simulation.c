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

bool init_single_philosopher(t_philosopher *philosopher, int index, pthread_mutex_t *left_fork, pthread_mutex_t *right_fork)
{
	// if (pthread_create(&philosopher->thread))
	// 	return (false);
	philosopher->index		= index;
	philosopher->left_fork	= left_fork;
	philosopher->right_fork	= right_fork;
	philosopher->state		= THINKING;
	philosopher->last_meal	= 0;
	philosopher->last_sleep	= 0;
	return (true);
}

t_philosopher *init_philosophers(int number_of_philosophers, pthread_mutex_t *forks)
{
	int index;
	t_philosopher *philosophers;

	index = 0;
	philosophers = malloc(sizeof(t_philosopher) * number_of_philosophers);
	if (!philosophers)
		return (NULL);
	while (index < number_of_philosophers)
	{
		if (!init_single_philosopher(&philosophers[index], index, &forks[index], &forks[(index + 1) % number_of_philosophers]))
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

void *routine(void *arg)
{
	t_philosopher *philosopher;

	philosopher = arg;
	printf("- Hello from philosopher number [%2d]\n",  philosopher->index);
	sleep(3);
	printf("- NOW I AM [%2d]\n",  philosopher->index);
	return (NULL);
}

bool launch_threads(t_philosopher *philosophers, int number_of_philosophers)
{
	int index;

	index = 0;
	while (index < number_of_philosophers)
	{
		if (pthread_create(&philosophers[index].thread, NULL, routine, &philosophers[index]))
			return (false);
		index++;
	}
	return (true);
}

void wait_threads(t_philosopher *philosophers, int number_of_philosophers)
{
	int index;

	index = 0;
	while (index < number_of_philosophers)
	{
		pthread_join(philosophers[index].thread, NULL);
		index++;
	}
}

unsigned int launch_simulation(t_simulation *simulation)
{
	t_philosopher	*philosophers;
	pthread_mutex_t	*forks;

	forks = init_forks(simulation->number_of_philosophers);
	if (!forks)
		return (ERR_COULD_NOT_INITIALIZE_FORKS);
	philosophers = init_philosophers(simulation->number_of_philosophers, forks);
	if (!philosophers)
	{
		destroy_forks(forks, simulation->number_of_philosophers);
		return (ERR_COULD_NOT_INITIALIZE_PHILOS);
	}
	// for (int i = 0; i < simulation->number_of_philosophers; i++)
	// 	printf("------- Number %02d -------\n- Fork L = %p\n- Fork R = %p\n- State = %d\n-------------------------\n", philosophers[i].index, philosophers[i].left_fork, philosophers[i].right_fork, philosophers[i].state);
	if (launch_threads(philosophers, simulation->number_of_philosophers))
		return (ERR_COULD_NOT_CREATE_THREAD);
	wait_threads(philosophers, simulation->number_of_philosophers);
	return (0);
}