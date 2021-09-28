#include "header.h"

void sleep_until_next_action(t_philosopher *philosopher)
{
	t_state			state;
	unsigned long	current_time;
	unsigned long	next_death;
	unsigned long	next_action;

	state = philosopher->state;
	current_time = get_current_time();
	next_death = philosopher->last_meal + philosopher->time_to_die;

	next_action = next_death;
	if (state == SLEEPING && current_time + philosopher->time_to_sleep <= next_death)
		next_action = current_time + philosopher->time_to_sleep;
	else if (state == EATING && current_time + philosopher->time_to_eat <= next_death)
		next_action = current_time + philosopher->time_to_eat;
	ft_msleep(next_action - current_time);
}

unsigned long	get_current_time(void)
{
	struct timeval	current_time;

	if (gettimeofday(&current_time, NULL))
		return (0);
	return (timeval_to_msec(&current_time));
}

unsigned long get_elapsed_time(t_philosopher *philosopher)
{
	return (get_current_time() - philosopher->starting_time);
}

unsigned long	timeval_to_msec(struct timeval *time)
{
	return (time->tv_sec * 1000 + time->tv_usec / 1000);
}

void ft_msleep(unsigned long duration)
{
	unsigned long target_time;

	target_time = get_current_time() + duration;
	while (get_current_time() <= target_time)
		usleep(10);
}
