#include "header.h"

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
	for (int i = 0; i < simulation->number_of_philosophers; i++)
		printf("------- Number %02d -------\n- Fork L = %p\n- Fork R = %p\n- State = %d\n- Time = %lu\n-------------------------\n", philosophers[i].index, philosophers[i].left_fork, philosophers[i].right_fork, philosophers[i].state, philosophers[i].starting_time);
	if (!launch_threads(philosophers, simulation->number_of_philosophers))
		return (ERR_COULD_NOT_CREATE_THREAD);
	wait_threads(philosophers, simulation->number_of_philosophers);
	return (SUCCESS);
}
