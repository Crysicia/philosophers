#include <pthread.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <errno.h>
#include <string.h>
#include <limits.h>
#include "structs.h"
#include "initialization.h"
#include "destruction.h"

void	ft_bzero(void *s, size_t n)
{
	size_t index;

	index = 0;
	while (index < n)
	{
		*(char *)(s + index) = 0;
		index++;
	}
}

void 			display_message(size_t id, t_state state);
void 			*routine(void *arg);
int				ft_atoi(const char *str);

int ft_atoi(const char *str)
{
	int total;

	total = 0;
	if (!str)
		return (-1);
	while (*str)
	{
		if (*str < '0' || *str > '9')
			return (-1);
		total += (total * 10) + *str - '0';
		if (total < 0)
			return (-1);
		str++;
	}
	return (total);
}

// t_simulation *init_simulation(size_t number_of_philosophers)
// {
// 	t_simulation *simulation;

// 	simulation = malloc(sizeof(t_simulation));
// 	if (!simulation)
// 		return (NULL);
// 	simulation->number_of_philosophers = 0; // BEWARE, if philosophers are initialized but not forks, philos WILL NOT BE freed
// 	simulation->philosophers = init_philosophers(number_of_philosophers);
// 	simulation->forks = init_forks(number_of_philosophers);
// 	if (!simulation->philosophers || !simulation->forks)
// 		return (destroy_simulation(simulation));
// 	simulation->number_of_philosophers = number_of_philosophers;
// 	return (simulation);
// }

void display_message(size_t id, t_state state)
{
	struct timeval	current_time;
	static char		*messages[5] = { "is thinking", "has taken a fork", "is eating", "is sleeping", "died" };

	gettimeofday(&current_time, NULL);
	printf("%ld: %zu %s\n", current_time.tv_sec, id, messages[state]);
}

void *routine(void *arg)
{
	t_philosopher *philo;

	philo = (t_philosopher *)arg;
	printf("%0.2zu -> says howdy to you, stranger\n", philo->index);
	usleep(100);
	philo->state = DEAD;
	return (arg);
}

int watcher(t_simulation *simulation)
{
	size_t index;

	while (true)
	{
		index = 0;
		while (index < simulation->number_of_philosophers)
		{
			printf("[%lu] - State: %d\n", index, ((t_philosopher *)simulation->philosophers[index])->state);
			if (((t_philosopher *)simulation->philosophers[index])->state == DEAD)
			{
				display_message(index, DEAD);
				return (ERROR);
			}
			index++;
		}
	}
	return (SUCCESS);
}

// To really destroy threads we should unlock all forks then check if the philo is dead, then return from routine and the thread will detach itself

int main(int argc, char *argv[])
{
	t_simulation *simulation;
	t_constants *constants;
	size_t index = 0;
	int number_of_philosophers;

	if (argc < 2)
		return (1);
	simulation = init_simulation(argc, argv);
	if (!simulation)
		printf("ERROR: couldn't initialize simulation\n");
	watcher(simulation);
	while (index < simulation->number_of_philosophers)
	{
		int ret = pthread_join(((t_philosopher *)simulation->philosophers[index])->thread, NULL);
		if (ret != 0)
			printf("Error joining threads: %s\n", strerror(errno));
		index++;
	}
	destroy_simulation(simulation);
	return (0);
}