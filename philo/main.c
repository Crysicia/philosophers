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

void 	display_message(t_simulation *simulation, size_t id, t_state state);
void 	*routine(void *arg);
int		ft_atoi(const char *str);
int		watcher(t_simulation *simulation);

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
		total = (total * 10) + (*str - '0');
		if (total < 0)
			return (-1);
		str++;
	}
	return (total);
}

void display_message(t_simulation *simulation, size_t id, t_state state)
{
	static char		*messages[5] = { "is thinking", "has taken a fork", "is eating", "is sleeping", "died" };

	printf("%lu: %zu %s\n", get_elapsed_time(simulation), id, messages[state]);
}

// CPETE
void *routine(void *arg)
{
	t_philosopher *philo;

	philo = (t_philosopher *)arg;
	printf("%0.2zu -> says howdy to you, stranger\n", philo->index);
	if (philo->index % 2)
		philo->state = HUNGRY;
	while (philo->state != DEAD)
	{
		if (philo->state == HUNGRY && philo->last_meal < get_current_time() - 999)
		{
			philo->state = EATING;
			philo->last_meal = get_current_time();
			if (!philo->last_meal)
				printf("ERROR getting current time\n");
		}
		else if (philo->state == HUNGRY && philo->last_meal > get_current_time() + 2000)
			philo->state = DEAD;
		else
			philo->state = HUNGRY;
	}
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
				display_message(simulation, index, DEAD);
				return (ERROR);
			}
			index++;
		}
		usleep(100);
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
	{
		printf("ERROR: couldn't initialize simulation\n");
		return (1);
	}
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
