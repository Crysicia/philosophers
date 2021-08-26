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
	// static char		*messages[5] = { "is thinking", "has taken a fork", "is eating", "is sleeping", "died" };
	static char		*messages[4] = { "is thinking\n", "is eating\n", "is sleeping\n", "died\n" };
	// static int		str_len[4] = { 12, 10, 12, 5 };

	printf("%lu %zu %s", get_elapsed_time(simulation), id + 1, messages[state]);
}

bool philo_can_eat(t_philosopher *philosopher)
{
	int time_to_die;

	time_to_die = ((t_constants *)((t_simulation *)philosopher->simulation)->constants)->time_to_die;
	if (philosopher->state == THINKING)
		return (true);
	return (false);
}

bool philo_is_starving(t_philosopher *philosopher)
{
	int time_to_die;
	unsigned long starting_time;
	unsigned long current_time;

	time_to_die = ((t_constants *)((t_simulation *)philosopher->simulation)->constants)->time_to_die;
	starting_time = ((t_simulation *)philosopher->simulation)->starting_time;
	current_time = get_current_time();
	// if (((philosopher->state == THINKING || philosopher->state == SLEEPING)
	// 		&& philosopher->last_meal + time_to_die <= get_current_time()) && philosopher->last_meal != 0)
	// 	return (true);
	if (((philosopher->last_meal + time_to_die <= current_time) && philosopher->last_meal != 0)
		|| (philosopher->last_meal == 0 && starting_time + time_to_die <= current_time))
		return (true);
	return (false);
}

bool philo_has_eaten(t_philosopher *philosopher)
{
	int time_to_eat;

	time_to_eat = ((t_constants *)((t_simulation *)philosopher->simulation)->constants)->time_to_eat;
	if (philosopher->state == EATING && philosopher->last_meal + time_to_eat <= get_current_time())
		return (true);
	return (false);
}

void philo_eat(t_philosopher *philosopher)
{
	pthread_mutex_lock(philosopher->left_fork);
	pthread_mutex_lock(philosopher->right_fork);
	if (!((t_simulation *)philosopher->simulation)->running)
	{
		pthread_mutex_unlock(philosopher->right_fork);
		pthread_mutex_unlock(philosopher->left_fork);
		return ;
	}
	philosopher->last_meal = get_current_time();
	philosopher->state = EATING;
	display_message(philosopher->simulation, philosopher->index, philosopher->state);
}

void philo_sleep(t_philosopher *philosopher)
{
	pthread_mutex_unlock(philosopher->right_fork);
	pthread_mutex_unlock(philosopher->left_fork);
	philosopher->last_sleep = get_current_time();
	philosopher->state = SLEEPING;
	display_message(philosopher->simulation, philosopher->index, philosopher->state);
}

void philo_kill(t_philosopher *philosopher)
{
	philosopher->state = DEAD;
	// display_message(philosopher->simulation, philosopher->index, philosopher->state);
}

void unlock_all_forks(t_simulation *simulation)
{
	int index;

	index = 0;
	while (index < simulation->number_of_philosophers)
	{
		pthread_mutex_unlock(&simulation->forks[index]);
		index++;
	}
}

void *routine(void *arg)
{
	int time_to_sleep;
	t_philosopher *philo;

	philo = (t_philosopher *)arg;

	time_to_sleep = ((t_constants *)((t_simulation *)philo->simulation)->constants)->time_to_sleep;
	if (philo->index % 2)
		usleep(time_to_sleep);
	// printf("%0.2zu -> says howdy to you, stranger\n", philo->index);
	// if (philo->index % 2)
	// 	philo->state = THINKING;
	while (philo->state != DEAD && ((t_simulation *)philo->simulation)->running)
	{
		// if (philo_is_starving(philo))
		// 	philo_kill(philo);
		if (philo_can_eat(philo))
			philo_eat(philo);
		else if (philo_has_eaten(philo))
			philo_sleep(philo);
		else if (philo->state == SLEEPING && philo->last_sleep + time_to_sleep < get_current_time())
		{
			philo->state = THINKING;
			display_message(philo->simulation, philo->index, philo->state);
		}
		usleep(100);
	}
	return (arg);
}

int watcher(t_simulation *simulation)
{
	int index;

	while (true)
	{
		index = 0;
		while (index < simulation->number_of_philosophers)
		{
			// printf("[%lu] - State: %d\n", index, ((t_philosopher *)simulation->philosophers[index])->state);
			if (philo_is_starving((t_philosopher *)simulation->philosophers[index]))
			{
				display_message(simulation, index, DEAD);
				simulation->running = false;
				unlock_all_forks(simulation);
				exit(42);
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
	int index;

	index = 0;
	if (argc < 2)
		return (1);
	simulation = init_simulation(argv);
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
