#include "time.h"

unsigned long get_elapsed_time(t_simulation *simulation)
{
	struct timeval current_time;

	gettimeofday(&current_time, NULL);
	return (timeval_to_msec(&current_time) - simulation->starting_time);
}

unsigned long	get_current_time(void)
{
	struct timeval	current_time;

	if (gettimeofday(&current_time, NULL))
		return (0);
	return (timeval_to_msec(&current_time));
}

unsigned long	timeval_to_msec(struct timeval *time)
{
	return (time->tv_sec * 1000 + time->tv_usec / 1000);
}
