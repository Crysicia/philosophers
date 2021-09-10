#ifndef HEADER_H
# define HEADER_H

#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>
#include <stdio.h>
#include <stdbool.h>

typedef enum	e_state { THINKING, FORK, EATING, SLEEPING, DEAD } t_state;
typedef struct	s_simulation
{
	int number_of_philosophers;
	int time_to_die;
	int time_to_eat;
	int time_to_sleep;
	int number_of_meals;
}				t_simulation;
typedef struct	s_philosopher
{
	int				index;
	t_state 		state; 
	pthread_t		thread;
	unsigned long	last_meal; 
	unsigned long	last_sleep;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
}				t_philosopher;

#define NUMBER_OF_PHILOSOPHERS	1
#define TIME_TO_DIE				2
#define TIME_TO_EAT				3
#define TIME_TO_SLEEP			4
#define NUMBER_OF_MEALS			5

// ---- ERRORS ----
#define NUMBER_OF_ERRORS				3

#define SUCCESS							0
#define ERR_COULD_NOT_INITIALIZE_PHILOS 1
#define ERR_COULD_NOT_INITIALIZE_FORKS	2
#define ERR_COULD_NOT_CREATE_THREAD		3

#define MSG_COULD_NOT_INITIALIZE_PHILOS	"Could not initialize philosophers"
#define MSG_COULD_NOT_INITIALIZE_FORKS	"Could not initialize forks"
#define MSG_COULD_NOT_CREATE_THREAD 	"Could not create thread"


// ---- PARSING ----
t_simulation *parse_arguments(int argc, char *argv[]);

// ---- SIMULATION ----
unsigned int launch_simulation(t_simulation *simulation);

#endif