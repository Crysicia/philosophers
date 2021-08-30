#ifndef INITIALIZATION_H
#define INITIALIZATION_H

#include "destruction.h"
#include "structs.h"
#include "time.h"
#include <pthread.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/time.h>

void			*routine(void *arg);

t_simulation	*init_simulation(char *argv[]);
bool			init_philosopher(t_simulation *simulation, t_philosopher **philosopher, size_t index);
pthread_mutex_t	*init_forks(size_t number_of_forks);
t_philosopher	**init_philosophers(t_simulation *simulation, size_t number_of_philosophers);

#endif
