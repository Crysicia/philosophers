#include <pthread.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <errno.h>
#include <string.h>

#define SUCCESS	0
#define ERROR	1

typedef enum	e_state { THINKING, HUNGRY, EATING, SLEEPING, DEAD } t_state;

typedef struct	s_philosopher
{
	size_t			index;
	t_state			state;
	pthread_t		thread;
	unsigned int	last_meal;
}				t_philosopher;

typedef struct	s_simulation
{
	size_t			number_of_philosophers;
	t_philosopher	*philosophers;
	pthread_mutex_t	*forks;
}				t_simulation;

pthread_mutex_t *init_forks(int number_of_philosophers);
bool init_philosopher(t_philosopher *philosopher, size_t index);
void display_message(size_t id, t_state state);
void *routine(void *arg);
void *destroy_simulation(t_simulation *simulation);
void *destroy_forks(pthread_mutex_t *forks, size_t number_of_forks);
void *destroy_philosophers(t_philosopher *philosophers, size_t number_of_philosophers);
t_philosopher *init_philosophers(size_t number_of_philosophers);

t_simulation *init_simulation(int number_of_philosophers)
{
	t_simulation *simulation;

	simulation = malloc(sizeof(t_simulation));
	if (!simulation)
		return (NULL);
	simulation->number_of_philosophers = 0;
	simulation->philosophers = init_philosophers(number_of_philosophers);
	simulation->forks = init_forks(number_of_philosophers);
	if (!simulation->philosophers || !simulation->forks)
		return (destroy_simulation(simulation));
	simulation->number_of_philosophers = number_of_philosophers;
	return (simulation);
}

void *destroy_simulation(t_simulation *simulation)
{
	destroy_forks(simulation->forks, simulation->number_of_philosophers);
	destroy_philosophers(simulation->philosophers, simulation->number_of_philosophers);

	return (NULL);
}

void *destroy_forks(pthread_mutex_t *forks, size_t number_of_forks)
{
	size_t index;

	index = 0;
	while (index < number_of_forks)
	{
		pthread_mutex_destroy(&forks[index]);
		index++;
	}
	free(forks);
	return (NULL);
}

pthread_mutex_t *init_forks(int number_of_philosophers)
{
	pthread_mutex_t *forks;
	size_t index;

	index = 0;
	forks = malloc(sizeof(pthread_mutex_t) * number_of_philosophers);
	if (!forks)
		return (NULL);
	while (index < number_of_philosophers)
	{
		if (!pthread_mutex_init(&forks[index], NULL))
			return (destroy_forks(forks, index));
		index++;
	}
	return (forks);
}

t_philosopher *init_philosophers(size_t number_of_philosophers)
{
	t_philosopher *philosophers;
	size_t index;
 
	index = 0;
	philosophers = malloc(sizeof(t_philosopher) * number_of_philosophers);
	if (!philosophers)
		return (NULL);
	while (index < number_of_philosophers)
	{
		if (!init_philosopher(&philosophers[index], index))
			return (destroy_philosophers(philosophers, index));
		index++;
	}
	return (philosophers);
}

void *destroy_philosophers(t_philosopher *philosophers, size_t number_of_philosophers)
{
	size_t index;

	index = 0;
	while (index < number_of_philosophers)
	{
		pthread_detach(philosophers[index].thread);
		philosophers[index].state = DEAD;
		index++;
	}
	free(philosophers);
	return (NULL);
}

bool init_philosopher(t_philosopher *philosopher, size_t index)
{
	philosopher->state = THINKING;
	philosopher->index = index;
	if (pthread_create(&philosopher->thread, NULL, &routine, &philosopher->index) != 0)
		return (false);
	display_message(index, THINKING);
	return (true);
}

void display_message(size_t id, t_state state)
{
	struct timeval current_time;
	static char *messages[5] = { "is thinking", "has taken a fork", "is eating", "is sleeping", "died" };

	gettimeofday(&current_time, NULL);
	printf("%ld: %zu %s\n", current_time.tv_sec, id, messages[state]);
}

void *routine(void *arg)
{
	size_t id;

	id = *(size_t *)arg;
	printf("%0.2zu -> says howdy to you, stranger\n", id);
	return (arg);
}

int watcher(t_simulation *simulation)
{
	return (SUCCESS);
}

int main(int argc, char *argv[])
{
	t_simulation simulation;
	t_philosopher *philosophers;
	
	size_t index = 0;
	int number_of_philosophers;

	if (argc < 2)
		return (1);
	number_of_philosophers = atoi(argv[1]);
	if (number_of_philosophers <= 0)
		return (2);
	simulation.number_of_philosophers = (size_t)number_of_philosophers;
	simulation.philosophers = init_philosophers(simulation.number_of_philosophers);
	while (index < simulation.number_of_philosophers)
	{
		int ret = pthread_join(simulation.philosophers[index].thread, NULL);
		if (ret != 0)
			printf("Error joining threads: %s\n", strerror(errno));
		index++;
	}
	return (0);
}