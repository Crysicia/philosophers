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

#define NUMBER_OF_PHILOSOPHERS 1
#define TIME_TO_DIE 2
#define TIME_TO_EAT 3
#define TIME_TO_SLEEP 4
#define NUMBER_OF_MEALS 5

typedef enum	e_state { THINKING, HUNGRY, EATING, SLEEPING, DEAD } t_state;
// typedef enum	e_error { SUCCESS, ERR_WRONG_NUMBER_OF_ARGUMENTS, ERR_ARGUMENT_OUT_OF_BOUNDS } t_error;

typedef struct	s_constants
{
	int	time_to_eat;
	int	time_to_sleep;
	int	time_to_die;
}				t_constants;

typedef struct	s_philosopher
{
	size_t			index;
	t_state			state;
	pthread_t		thread;
	unsigned int	last_meal;
	unsigned int	meal_count;
}				t_philosopher;

typedef struct	s_simulation
{
	t_constants		*constants;
	int				number_of_philosophers;
	t_philosopher	**philosophers;
	pthread_mutex_t	*forks;
}				t_simulation;


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

pthread_mutex_t	*init_forks(size_t number_of_philosophers);
bool			init_philosopher(t_philosopher **philosopher, size_t index);
void 			display_message(size_t id, t_state state);
void 			*routine(void *arg);
void 			*destroy_simulation(t_simulation *simulation);
void 			*destroy_forks(pthread_mutex_t *forks);
void 			*destroy_philosophers(t_philosopher **philosophers);
t_philosopher	**init_philosophers(size_t number_of_philosophers);
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

t_simulation *init_simulation(int argc, char *argv[])
{
	t_simulation *simulation;

	simulation = malloc(sizeof(t_simulation));
	if (!simulation)
		return (NULL);
	ft_bzero(simulation, sizeof(t_simulation));
	simulation->constants = init_constants(argc, argv);
	simulation->number_of_philosophers = ft_atoi(argv[NUMBER_OF_PHILOSOPHERS]);
	if (simulation->number_of_philosophers == -1 || !simulation->constants)
		return (destroy_simulation(simulation));
	simulation->philosophers = init_philosophers(simulation->number_of_philosophers);
	simulation->forks = init_forks(simulation->number_of_philosophers);
	if (!simulation->philosophers || !simulation->forks)
		return (destroy_simulation(simulation));
	return (simulation);
}

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

void *destroy_philosophers(t_philosopher **philosophers)
{
	size_t index;

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

bool init_philosopher(t_philosopher **philosopher, size_t index)
{
	*philosopher = malloc(sizeof(t_philosopher));
	printf("CALLED INIT PHILO\n");
	if (!philosopher)
		return (NULL);
	(*philosopher)->state = THINKING;
	(*philosopher)->index = index;
	if (pthread_create(&(*philosopher)->thread, NULL, &routine, *philosopher) != 0)
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