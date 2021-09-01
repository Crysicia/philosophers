#include "actions.h"

void philo_sleep(t_philosopher *philosopher)
{
	int time_to_sleep;

	time_to_sleep = ((t_simulation *)philosopher->simulation)->time_to_sleep;
	philo_put_fork(philosopher->left_fork);
	philo_put_fork(philosopher->right_fork);
	philosopher->last_sleep = get_current_time();
	philosopher->state = SLEEPING;
	display_message(philosopher->simulation, philosopher->index, philosopher->state);
	ft_usleep((time_to_sleep * 1000) - 10);
}

void philo_eat(t_philosopher *philosopher)
{
	int time_to_eat;

	time_to_eat = ((t_simulation *)philosopher->simulation)->time_to_eat;
	philo_take_fork(philosopher, philosopher->left_fork);
	philo_take_fork(philosopher, philosopher->right_fork);
	if (!((t_simulation *)philosopher->simulation)->running)
	{
		philo_put_fork(philosopher->right_fork);
		philo_put_fork(philosopher->left_fork);
		return ;
	}
	philosopher->last_meal = get_current_time();
	philosopher->state = EATING;
	display_message(philosopher->simulation, philosopher->index, philosopher->state);
	ft_usleep((time_to_eat * 1000) - 10);
}

bool philo_has_eaten(t_philosopher *philosopher, unsigned long current_time)
{
	int time_to_eat;

	time_to_eat = ((t_simulation *)philosopher->simulation)->time_to_eat;
	if (philosopher->state == EATING && philosopher->last_meal + time_to_eat <= current_time)
		return (true);
	return (false);
}

bool philo_is_starving(t_philosopher *philosopher, unsigned long current_time)
{
	int time_to_die;
	unsigned long starting_time;
	unsigned long last_meal;

	time_to_die = ((t_simulation *)philosopher->simulation)->time_to_die;
	starting_time = ((t_simulation *)philosopher->simulation)->starting_time;
	last_meal = philosopher->last_meal;
	if (((last_meal + time_to_die <= current_time) && last_meal != 0)
		|| (last_meal == 0 && starting_time + time_to_die <= current_time))
		return (true);
	return (false);
}

bool philo_can_eat(t_philosopher *philosopher)
{
	if (philosopher->state == THINKING)
		return (true);
	return (false);
}
