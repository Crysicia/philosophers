#include "time.h"

unsigned long	get_current_time(void)
{
	struct timeval	current_time;

	if (gettimeofday(&current_time, NULL))
		return (0);
	return (timeval_to_msec(&current_time));
}

unsigned long get_elapsed_time(t_simulation *simulation)
{
	return (get_current_time() - simulation->starting_time);
}

unsigned long	timeval_to_msec(struct timeval *time)
{
	return (time->tv_sec * 1000 + time->tv_usec / 1000);
}

void ft_usleep(unsigned long duration)
{
	unsigned long target_time;

	target_time = (get_current_time() * 1000) + duration;
	while (get_current_time() * 1000 <= target_time)
		usleep(100);
}
