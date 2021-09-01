#ifndef ACTIONS_H
#define ACTIONS_H

#include "destruction.h"
#include "everything.h"
#include "structs.h"
#include "time.h"
#include <pthread.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/time.h>

void philo_sleep(t_philosopher *philosopher);
void philo_eat(t_philosopher *philosopher);
bool philo_has_eaten(t_philosopher *philosopher, unsigned long current_time);
bool philo_is_starving(t_philosopher *philosopher, unsigned long current_time);

#endif
