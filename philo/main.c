#include <pthread.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <errno.h>
#include <string.h>
#include <limits.h>

#define SUCCESS	0
#define ERROR	1
#define ERR_WRONG_NUMBER_OF_ARGUMENTS	2
#define ERR_ARGUMENT_OUT_OF_BOUNDS	3
#define ERR_MALLOC_FAILED	4

typedef enum	e_state { THINKING, HUNGRY, EATING, SLEEPING, DEAD } t_state;
// typedef enum	e_error { SUCCESS, ERR_WRONG_NUMBER_OF_ARGUMENTS, ERR_ARGUMENT_OUT_OF_BOUNDS } t_error;

typedef struct	s_constants
{
	unsigned int	time_to_eat;
	unsigned int	time_to_sleep;
	unsigned int	time_to_die;
}				t_constants;

typedef struct	s_philosopher
{
	size_t			index;
	t_state			state;
	pthread_t		thread;
	unsigned int	last_meal;
	unsigned int	meal_count;
	t_constants		*constants;
}				t_philosopher;

typedef struct	s_simulation
{
	t_constants		*constants;
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

long	ft_atol(const char *str)
{
	long long	total;
	int			sign;
	size_t		index;

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

bool is_in_bounds(long value, long lower, long upper)
{
	if (value < lower || value > upper)
		return (false);
	return (true);
}

#define NUMBER_OF_PHILOSOPHERS 1
#define TIME_TO_DIE 2
#define TIME_TO_EAT 3
#define TIME_TO_SLEEP 4
#define NUMBER_OF_MEALS 5

int parse_arguments(t_constants *constants, int *nop, char *argv[], int argc)
{
	long			number_of_philosophers;
	long			time_to_eat;
	long			time_to_sleep;
	long			time_to_die;

	if (!constants)
		return (ERROR);
	if (argc != 5 && argc != 6)
		return (ERR_WRONG_NUMBER_OF_ARGUMENTS);
	number_of_philosophers = ft_atol(argv[NUMBER_OF_PHILOSOPHERS]);
	if (!is_in_bounds(number_of_philosophers, 0, INT_MAX))
		return (ERR_ARGUMENT_OUT_OF_BOUNDS);
	time_to_die = ft_atol(argv[TIME_TO_DIE]);
	if (!is_in_bounds(time_to_die, 0, INT_MAX))
		return (ERR_ARGUMENT_OUT_OF_BOUNDS);
	time_to_eat = ft_atol(argv[TIME_TO_EAT]);
	if (!is_in_bounds(time_to_eat, 0, INT_MAX))
		return (ERR_ARGUMENT_OUT_OF_BOUNDS);
	time_to_sleep = ft_atol(argv[TIME_TO_SLEEP]);
	if (!is_in_bounds(time_to_sleep, 0, INT_MAX))
		return (ERR_ARGUMENT_OUT_OF_BOUNDS);
	if (argc == 6)
	{
		printf("Number of meals not implemented yet !\n");
		// time_to_sleep = ft_atol(argv[TIME_TO_SLEEP]);
		// if (!is_in_bounds(time_to_sleep, 0, INT_MAX))
		// 	return (ERR_ARGUMENT_OUT_OF_BOUNDS);
	}
	constants->time_to_die = time_to_die;
	constants->time_to_eat = time_to_eat;
	constants->time_to_sleep = time_to_sleep;
	*nop = number_of_philosophers;

	return (SUCCESS);
}

// To really destroy threads we should unlock all forks then check if the philo is dead, then return from routine and the thread will detach itself

int main(int argc, char *argv[])
{
	t_simulation *simulation;
	t_philosopher *philosophers;
	t_constants *constants;
	size_t index = 0;
	int number_of_philosophers;

	if (argc < 2)
		return (1);
	constants = malloc(sizeof(t_constants));
	if (!constants)
		return (2);
	if (parse_arguments(constants, &number_of_philosophers, argv, argc) != SUCCESS)
	{
		printf("Error while parsing arguments\n");
		return (3);
	}
	simulation = init_simulation(number_of_philosophers);
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