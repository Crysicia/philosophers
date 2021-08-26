#ifndef STRUCTS_H
#define STRUCTS_H

#include <pthread.h>
#include <stdbool.h>

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
#define USAGE "philo [number_of_philosophers] [time_to_die] [time_to_eat] [time_to_sleep] <number_of_meal>"

#define T_TO_DIE 500
#define T_TO_EAT 300
#define T_TO_SLEEP 3000

typedef enum	e_state { THINKING, EATING, SLEEPING, DEAD } t_state;
// typedef enum	e_error { SUCCESS, ERR_WRONG_NUMBER_OF_ARGUMENTS, ERR_ARGUMENT_OUT_OF_BOUNDS } t_error;
typedef enum	e_fork_placement { LEFT = -1, RIGHT = 0 } t_fork_placement;

typedef struct	s_simulation t_simulation;

typedef struct	s_constants
{
	int	time_to_eat;
	int	time_to_sleep;
	int	time_to_die;
}				t_constants;

typedef struct	s_philosopher
{
	t_simulation	*simulation;
	size_t			index;
	t_state			state;
	pthread_t		thread;
	unsigned long	last_meal;
	unsigned long	last_sleep;
	unsigned long	meal_count;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
}				t_philosopher;

typedef struct	s_simulation
{
	t_constants			*constants;
	int					number_of_philosophers;
	t_philosopher		**philosophers;
	pthread_mutex_t		*forks;
	unsigned long		starting_time;
	bool				running;
}				t_simulation;

#endif
