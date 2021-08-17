#ifndef DESTRUCTION_H
#define DESTRUCTION_H

#include <pthread.h>
#include "structs.h"

void *destroy_simulation(t_simulation *simulation);
void *destroy_forks(pthread_mutex_t *forks);
void *destroy_philosophers(t_philosopher **philosophers);

#endif DESTRUCTION_H