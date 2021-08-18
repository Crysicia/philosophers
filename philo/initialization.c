#include "initialization.h"

void			ft_bzero(void *s, size_t n);

t_constants *init_constants(int argc, char *argv[])
{
	t_constants *constants;

	constants = malloc(sizeof(t_constants));
	if (!constants)
		return (NULL);
	constants->time_to_die = ft_atoi(argv[TIME_TO_DIE]);
	constants->time_to_eat = ft_atoi(argv[TIME_TO_EAT]);
	constants->time_to_sleep = ft_atoi(argv[TIME_TO_SLEEP]);
	if (constants->time_to_die == -1
		|| constants->time_to_eat == -1
		|| constants->time_to_sleep == -1)
	{
		free(constants);
		return (NULL);
	}
	return (constants);
}

#include <stdio.h>
t_simulation *init_simulation(int argc, char *argv[])
{
	t_simulation	*simulation;
	struct timeval	current_time;

	simulation = malloc(sizeof(t_simulation));
	if (!simulation)
		return (NULL);
	ft_bzero(simulation, sizeof(t_simulation));
	simulation->constants = init_constants(argc, argv);
	simulation->number_of_philosophers = ft_atoi(argv[NUMBER_OF_PHILOSOPHERS]);
	if (simulation->number_of_philosophers == -1 || !simulation->constants || gettimeofday(&current_time, NULL))
		return (destroy_simulation(simulation));
	simulation->starting_time = timeval_to_msec(&current_time);
	simulation->philosophers = init_philosophers(simulation->number_of_philosophers);
	simulation->forks = init_forks(simulation->number_of_philosophers);
	if (!simulation->philosophers || !simulation->forks)
		return (destroy_simulation(simulation));
	return (simulation);
}

bool init_philosopher(t_philosopher **philosopher, size_t index)
{
	*philosopher = malloc(sizeof(t_philosopher));
	if (!philosopher)
		return (NULL);
	(*philosopher)->state = THINKING;
	(*philosopher)->index = index;
	if (pthread_create(&(*philosopher)->thread, NULL, &routine, *philosopher) != 0)
		return (false);
	return (true);
}

pthread_mutex_t *init_forks(size_t number_of_forks)
{
	pthread_mutex_t *forks;
	size_t index;

	index = 0;
	forks = malloc(sizeof(pthread_mutex_t) * (number_of_forks + 1));
	if (!forks)
		return (NULL);
	ft_bzero(forks, sizeof(pthread_mutex_t) * (number_of_forks + 1));
	while (index < number_of_forks)
	{
		if (pthread_mutex_init(&forks[index], NULL) != 0)
			return (destroy_forks(forks));
		index++;
	}
	return (forks);
}

t_philosopher **init_philosophers(size_t number_of_philosophers)
{
	t_philosopher **philosophers;
	size_t index;
 
	index = 0;
	philosophers = malloc(sizeof(t_philosopher *) * (number_of_philosophers + 1));
	if (!philosophers)
		return (NULL);
	ft_bzero(philosophers, sizeof(t_philosopher *) * (number_of_philosophers + 1));
	while (index < number_of_philosophers)
	{
		if (!init_philosopher(&(philosophers[index]), index))
			return (destroy_philosophers(philosophers));
		index++;
	}
	return (philosophers);
}