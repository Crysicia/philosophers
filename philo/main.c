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

typedef struct	s_timers
{
	unsigned int	time_to_eat;
	unsigned int	time_to_sleep;
	unsigned int	time_to_die;
}				t_timers;

typedef struct	s_philosopher
{
	size_t			index;
	t_state			state;
	pthread_t		thread;
	unsigned int	last_meal;
	unsigned int	meal_count;
	t_timers		*timers;
}				t_philosopher;

typedef struct	s_simulation
{
	size_t			number_of_philosophers;
	t_philosopher	*philosophers;
	pthread_mutex_t	*forks;
}				t_simulation;

pthread_mutex_t *init_forks(size_t number_of_philosophers);
bool init_philosopher(t_philosopher *philosopher, size_t index);
void display_message(size_t id, t_state state);
void *routine(void *arg);
void *destroy_simulation(t_simulation *simulation);
void *destroy_forks(pthread_mutex_t *forks, size_t number_of_forks);
void *destroy_philosophers(t_philosopher *philosophers, size_t number_of_philosophers);
t_philosopher *init_philosophers(size_t number_of_philosophers);
int	ft_atoi(const char *str);

int	ft_atoi(const char *str)
{
	long	total;
	int		sign;
	size_t	index;

	total = 0;
	sign = 1;
	index = 0;
	while ((str[index] >= 9 && 13 >= str[index]) || str[index] == 32)
		index++;
	if (str[index] == '-' || str[index] == '+')
	{
		if (str[index] == '-')
			sign = -1;
		index++;
	}
	while (str[index] >= '0' && '9' >= str[index])
	{
		total *= 10;
		total += str[index] - '0';
		index++;
	}
	return (total * sign);
}

t_simulation *init_simulation(size_t number_of_philosophers)
{
	t_simulation *simulation;

	simulation = malloc(sizeof(t_simulation));
	if (!simulation)
		return (NULL);
	simulation->number_of_philosophers = 0; // BEWARE, if philosophers are initialized but not forks, philos WILL NOT BE freed
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

pthread_mutex_t *init_forks(size_t number_of_philosophers)
{
	pthread_mutex_t *forks;
	size_t index;

	index = 0;
	forks = malloc(sizeof(pthread_mutex_t) * number_of_philosophers);
	if (!forks)
		return (NULL);
	while (index < number_of_philosophers)
	{
		if (pthread_mutex_init(&forks[index], NULL) != 0)
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
	if (pthread_create(&philosopher->thread, NULL, &routine, philosopher) != 0)
		return (false);
	return (true);
}

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
	printf("%0.2zu -> says howdy to you, stranger\n", philo->index);\
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
			printf("[%lu] - State: %d\n", index, simulation->philosophers[index].state);
			if (simulation->philosophers[index].state == DEAD)
			{
				display_message(index, DEAD);
				return (ERROR);
			}
			index++;
		}
	}
	return (SUCCESS);
}

bool parse_arguments(char *argv[])
{
	return (true);
}

// To really destroy threads we should unlock all forks then check if the philo is dead, then return from routine and the thread will detach itself

int main(int argc, char *argv[])
{
	t_simulation *simulation;
	t_philosopher *philosophers;

	size_t index = 0;
	int number_of_philosophers;

	if (argc < 2)
		return (1);
	number_of_philosophers = ft_atoi(argv[1]);
	if (number_of_philosophers <= 0)
		return (2);
	simulation = init_simulation((size_t)number_of_philosophers);
	if (!simulation)
		printf("ERROR: couldn't initialize simulation\n");
	watcher(simulation);
	while (index < simulation->number_of_philosophers)
	{
		int ret = pthread_join(simulation->philosophers[index].thread, NULL);
		if (ret != 0)
			printf("Error joining threads: %s\n", strerror(errno));
		index++;
	}
	destroy_simulation(simulation);
	return (0);
}