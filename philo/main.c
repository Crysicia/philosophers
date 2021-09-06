#include "everything.h"

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
void	philo_put_fork(pthread_mutex_t *fork);

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

unsigned long elapsed_time(unsigned long current_time, t_philosopher *philosopher)
{
	return (get_current_time() - ((t_simulation *)philosopher->simulation)->starting_time);
}

void new_display_message(t_philosopher *philosopher, unsigned long current_time)
{
	// static char		*messages[5] = { "is thinking", "has taken a fork", "is eating", "is sleeping", "died" };
	static char		*messages[5] = { "is thinking\n", "has taken a fork\n", "is eating\n", "is sleeping\n", "died\n" };
	// static int		str_len[5] = { 12, 17, 10, 12, 5 };
	// write(1, messages[state], str_len[state]);
	printf("%lu %zu %s", elapsed_time(current_time, philosopher), philosopher->index + 1, messages[philosopher->state]);
}

void display_message(t_simulation *simulation, size_t id, t_state state)
{
	// static char		*messages[5] = { "is thinking", "has taken a fork", "is eating", "is sleeping", "died" };
	static char		*messages[5] = { "is thinking\n", "has taken a fork\n", "is eating\n", "is sleeping\n", "died\n" };
	// static int		str_len[5] = { 12, 17, 10, 12, 5 };
	// write(1, messages[state], str_len[state]);
	printf("%lu %zu %s", get_elapsed_time(simulation), id + 1, messages[state]);
}

void philo_take_fork(t_philosopher *philosopher, pthread_mutex_t *fork)
{
	t_simulation *simulation;

	simulation = philosopher->simulation;
	if (!simulation->running)
		return ;
	pthread_mutex_lock(fork);
	if (!simulation->running)
		return ;
	new_display_message(philosopher, 0);
}

void philo_put_fork(pthread_mutex_t *fork)
{
	pthread_mutex_unlock(fork);
}

void philo_kill(t_philosopher *philosopher)
{
	// philosopher->state = DEAD;
	// ((t_simulation *)philosopher->simulation)->running = false;
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
	int				time_to_sleep;
	t_philosopher	*philo;
	unsigned long	current_time;

	philo = (t_philosopher *)arg;
	time_to_sleep = ((t_simulation *)philo->simulation)->time_to_sleep;
	if (philo->index % 2)
		ft_usleep(1000);
	while (philo->state != DEAD && ((t_simulation *)philo->simulation)->running)
	{
		current_time = get_current_time();
		if (philo_can_eat(philo))
			philo_eat(philo, current_time);
		else if (philo_has_eaten(philo, current_time))
			philo_sleep(philo, current_time);
		else if (philo->state == SLEEPING && philo->last_sleep + time_to_sleep <= current_time)
		{
			philo->state = THINKING;
			new_display_message(philo, current_time);
		}
		usleep(100);
	}
	return (arg);
}

int watcher(t_simulation *simulation)
{
	int index;
	unsigned long current_time;

	while (true)
	{
		index = 0;
		current_time = get_current_time();
		while (index < simulation->number_of_philosophers)
		{
			// printf("[%lu] - State: %d\n", index, ((t_philosopher *)simulation->philosophers[index])->state);
			if (philo_is_starving((t_philosopher *)simulation->philosophers[index], current_time))
			{
				simulation->running = false;
				display_message(simulation, index, DEAD);
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
	index = 0;
	while (index < simulation->number_of_philosophers)
	{
		pthread_create(&((t_philosopher *)simulation->philosophers[index])->thread, NULL, &routine, simulation->philosophers[index]);
		index++;
	}
	watcher(simulation);
	// pthread_create(&watcher_thread, NULL, watcher, simulation);
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
