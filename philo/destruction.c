#include "destruction.h"

void *destroy_simulation(t_simulation *simulation)
{
	destroy_forks(simulation->forks);
	destroy_philosophers(simulation->philosophers);
	return (NULL);
}

void *destroy_forks(pthread_mutex_t *forks)
{
	// size_t index;

	// index = 0;
	// while (&(forks + (sizeof(pthread_mutex_t) * index)))
	// {
	// 	pthread_mutex_destroy(&forks[index]);
	// 	index++;
	// }
	// free(forks);
	return (NULL);
}

void *destroy_philosophers(t_philosopher **philosophers)
{
	// size_t index;

	// index = 0;
	// while (0)
	// {
	// 	pthread_detach(philosophers[index].thread);
	// 	philosophers[index].state = DEAD;
	// 	index++;
	// }
	// free(philosophers);
	return (NULL);
}
