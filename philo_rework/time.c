/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpassera <lpassera@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/01 16:02:21 by lpassera          #+#    #+#             */
/*   Updated: 2021/10/01 16:11:49 by lpassera         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

unsigned long	get_current_time(void)
{
	struct timeval	current_time;

	if (gettimeofday(&current_time, NULL))
		return (0);
	return (timeval_to_msec(&current_time));
}

unsigned long	get_elapsed_time(t_philosopher *philosopher)
{
	return (get_current_time() - philosopher->simulation->starting_time);
}

unsigned long	timeval_to_msec(struct timeval *time)
{
	return (time->tv_sec * 1000 + time->tv_usec / 1000);
}

void	ft_msleep(unsigned long duration)
{
	unsigned long	target_time;

	target_time = get_current_time() + duration;
	while (get_current_time() < target_time)
		usleep(500);
}
