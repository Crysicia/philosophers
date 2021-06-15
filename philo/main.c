#include <pthread.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

typedef enum	e_state { THINKING, EATING, SLEEPING, DEAD } t_state;

typedef struct	s_philosopher
{
	size_t		index;
	t_state		state;
	pthread_t	thread;
}				t_philosopher;

bool init_philosopher(t_philosopher *philosopher, size_t index);
void *routine(void *arg);

t_philosopher *init_philosophers(int number_of_philosophers)
{
	t_philosopher *philosophers;
	int index;

	index = 0;
	philosophers = malloc(sizeof(t_philosopher) * number_of_philosophers);
	if (!philosophers)
		return (NULL);
	while (index < number_of_philosophers)
	{
		if (!init_philosopher(&philosophers[index], index))
		{
			free(philosophers);
			return (NULL);
		}
		index++;
	}
	return (philosophers);
}

bool init_philosopher(t_philosopher *philosopher, size_t index)
{
	philosopher->state = THINKING;
	philosopher->index = index;
	if (pthread_create(&philosopher->thread, NULL, &routine, &philosopher->index) != 0)
		return (false);
	return (true);
}

void *routine(void *arg)
{
	size_t id;

	id = *(size_t *)arg;
	printf("%0.2zu -> says howdy to you, stranger\n", id);
	return (arg);
}

int main(int argc, char *argv[])
{
	if (argc < 2)
		return (1);
	int number_of_philosophers = atoi(argv[1]);
	int index = 0;
	t_philosopher *philosophers;

	philosophers = init_philosophers(number_of_philosophers);
	while (index < number_of_philosophers)
	{
		pthread_join(philosophers[index].thread, NULL);
		index++;
	}
	return (0);
}