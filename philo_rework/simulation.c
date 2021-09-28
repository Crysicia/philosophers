#include "header.h"

void *routine(void *arg)
{
	bool is_first_iteration;
	t_philosopher *philosopher;

	is_first_iteration = true;
	philosopher = arg;
	// THIS IS BUGGY, IT'S WHYMY PHILOS AREDIYING LOOOL
	while (is_simulation_running(philosopher->simulation))
	{
		if ((philosopher->index % 2 && is_first_iteration) || !is_first_iteration)
			philo_eat(philosopher);
		philo_sleep(philosopher);
		philo_think(philosopher);
		is_first_iteration = false;
	}
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
		index += 1;
		// usleep(10);
	}
	// index = 1;
	// while (index < number_of_philosophers)
	// {
	// 	if (pthread_create(&philosophers[index].thread, NULL, routine, &philosophers[index]))
	// 		return (false);
	// 	index += 2;
	// }
	return (true);
}

void wait_threads(t_simulation *simulation)
{
	int index;

	index = 0;
	while (index < simulation->number_of_philosophers)
	{
		pthread_join(simulation->philosophers[index].thread, NULL);
		index++;
	}
	pthread_join(simulation->watcher, NULL);
}

bool launch_watcher(t_simulation *simulation)
{
	if (pthread_create(&simulation->watcher, NULL, watcher, simulation))
		return (false);
	return (true);
}

unsigned int launch_simulation(t_simulation *simulation)
{
	simulation->forks = init_forks(simulation->number_of_philosophers);
	if (!simulation->forks)
		return (ERR_COULD_NOT_INITIALIZE_FORKS);
	simulation->philosophers = init_philosophers(simulation, simulation->forks);
	if (!simulation->philosophers)
	{
		destroy_forks(simulation->forks, simulation->number_of_philosophers);
		return (ERR_COULD_NOT_INITIALIZE_PHILOS);
	}
	// for (int i = 0; i < simulation->number_of_philosophers; i++)
	// 	printf("------- Number %02d -------\n- Address: %p\n- Fork L = %p\n- Fork R = %p\n- State = %d\n- Time = %lu\n-------------------------\n", simulation->philosophers[i].index, &simulation->philosophers[i], simulation->philosophers[i].left_fork, simulation->philosophers[i].right_fork, simulation->philosophers[i].state, simulation->starting_time);
	if (!launch_threads(simulation->philosophers, simulation->number_of_philosophers))
		return (ERR_COULD_NOT_CREATE_THREAD);
	launch_watcher(simulation);
	wait_threads(simulation);
	return (SUCCESS);
}
