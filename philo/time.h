#ifndef TIME_H
#define TIME_H

#include "structs.h"
#include <sys/time.h>
#include <unistd.h>

unsigned long	get_elapsed_time(t_simulation *simulation);
unsigned long	get_current_time(void);
unsigned long	timeval_to_msec(struct timeval *time);
void			ft_usleep(unsigned long duration);

#endif
