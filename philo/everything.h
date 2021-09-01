#ifndef EVERYTHING_H
# define EVERYTHING_H

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
#include "actions.h"

void	ft_bzero(void *s, size_t n);
int ft_atoi(const char *str);
void ft_usleep(unsigned long duration);
void display_message(t_simulation *simulation, size_t id, t_state state);
void philo_take_fork(t_philosopher *philosopher, pthread_mutex_t *fork);
void philo_put_fork(pthread_mutex_t *fork);
void philo_kill(t_philosopher *philosopher);
void unlock_all_forks(t_simulation *simulation);
void wait_for_simulation(t_simulation *simulation);
void *routine(void *arg);
int watcher(t_simulation *simulation);

#endif
