#ifndef STRUCTS_H
#define STRUCTS_H

#include <pthread.h>

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

#endif