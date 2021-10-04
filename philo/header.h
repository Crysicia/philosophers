/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   header.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpassera <lpassera@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/01 16:10:14 by lpassera          #+#    #+#             */
/*   Updated: 2021/10/04 16:32:46 by lpassera         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEADER_H
# define HEADER_H

# include <unistd.h>
# include <stdlib.h>
# include <sys/time.h>
# include <pthread.h>
# include <stdio.h>
# include <stdbool.h>

typedef enum e_state { THINKING, FORK, EATING, SLEEPING, DEAD }	t_state;

typedef struct s_philosopher	t_philosopher;
typedef struct s_simulation
{
	int				number_of_philosophers;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				number_of_meals;
	int				meals_ate;
	bool			is_running;
	pthread_t		watcher;
	t_philosopher	*philosophers;
	unsigned long	starting_time;
	pthread_mutex_t	*forks;
	pthread_mutex_t	*write_lock;
	pthread_mutex_t	*access_lock;
}				t_simulation;

typedef struct s_philosopher
{
	int				index;
	int				number_of_meals;
	pthread_t		thread;
	t_simulation	*simulation;
	unsigned long	last_meal;
	unsigned long	last_sleep;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	pthread_mutex_t	*access_lock;
}				t_philosopher;

# define NB_OF_PHILO			1
# define TIME_TO_DIE			2
# define TIME_TO_EAT			3
# define TIME_TO_SLEEP			4
# define NUMBER_OF_MEALS		5

// ---- ERRORS ----
# define NUMBER_OF_ERRORS		3

# define SUCCESS							0
# define ERR_COULD_NOT_INITIALIZE_PHILOS 	1
# define ERR_COULD_NOT_INITIALIZE_FORKS		2
# define ERR_COULD_NOT_CREATE_THREAD		3

# define MSG_COULD_NOT_INITIALIZE_PHILOS	"Could not initialize philosophers"
# define MSG_COULD_NOT_INITIALIZE_FORKS		"Could not initialize forks"
# define MSG_COULD_NOT_CREATE_THREAD 		"Could not create thread"
# define MSG_UNEXPECTED_ERR			 		"Unexpected error"
# define MSG_USAGE							"<number_of_philosophers> \
<time_to_die> <time_to_eat> <time_to_sleep> [number_of_meals]"

void			destroy_simulation(t_simulation *simulation);

// ---- PARSING ----
t_simulation	*parse_arguments(int argc, char *argv[]);
pthread_mutex_t	*init_lock(void);
void			destroy_lock(pthread_mutex_t *lock);

// ---- SIMULATION ----
unsigned int	launch_simulation(t_simulation *simulation);

// ---- INIT ----
void			destroy_philosophers(t_philosopher *philosophers,
					int number_of_philosophers);
t_philosopher	*init_philosophers(t_simulation *simulation,
					pthread_mutex_t *forks);
void			destroy_forks(pthread_mutex_t *forks, int number_of_forks);
pthread_mutex_t	*init_forks(int number_of_forks);

// ---- ACTIONS----
void			philo_eat(t_philosopher *philosopher);
void			philo_sleep(t_philosopher *philosopher);
void			philo_think(t_philosopher *philosopher);
void			display_state(t_philosopher *philosopher, t_state state);

// ---- TIME ----
unsigned long	get_current_time(void);
unsigned long	get_elapsed_time(t_philosopher *philosopher);
unsigned long	timeval_to_msec(struct timeval *time);
void			ft_msleep(unsigned long duration);

// ---- WATCHER ----
void			*watcher(void *arg);
bool			is_simulation_running(t_simulation *simulation);
bool			philo_is_dead(t_philosopher *philosopher,
					unsigned int time_to_die);

// ---- ACCESS ----
void			increment_total_meals(t_simulation *simulation);
int				get_total_meals(t_simulation *simulation);
unsigned long	philo_get_duration(t_philosopher *philosopher,
					unsigned long *philo_duration);
void			philo_set_duration(t_philosopher *philosopher,
					unsigned long *philo_duration, unsigned long set_duration);

#endif
