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
void new_display_message(t_philosopher *philosopher, unsigned long current_time);


void philo_sleep(t_philosopher *philosopher, unsigned long current_time);
void philo_eat(t_philosopher *philosopher, unsigned long current_time);
bool philo_has_eaten(t_philosopher *philosopher, unsigned long current_time);
bool philo_is_starving(t_philosopher *philosopher, unsigned long current_time);
bool philo_can_eat(t_philosopher *philosopher);

#endif
