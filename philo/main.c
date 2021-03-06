/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpassera <lpassera@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/01 14:50:51 by lpassera          #+#    #+#             */
/*   Updated: 2021/10/04 16:30:55 by lpassera         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

unsigned int	print_err(unsigned int code)
{
	static char	*errors[NUMBER_OF_ERRORS] = {MSG_COULD_NOT_INITIALIZE_PHILOS,
		MSG_COULD_NOT_INITIALIZE_FORKS, MSG_COULD_NOT_CREATE_THREAD};
	char		*message;

	message = MSG_UNEXPECTED_ERR;
	if (code <= NUMBER_OF_ERRORS)
		message = errors[code - 1];
	printf("ERROR: %s\n", message);
	return (code);
}

void	destroy_simulation(t_simulation *simulation)
{
	destroy_lock(simulation->write_lock);
	destroy_lock(simulation->access_lock);
	free(simulation);
}

int	main(int argc, char *argv[])
{
	t_simulation	*simulation;
	unsigned int	ret;

	simulation = parse_arguments(argc, argv);
	if (!simulation)
	{
		printf("ERROR: Usage %s %s\n", argv[0], MSG_USAGE);
		return (1);
	}
	ret = launch_simulation(simulation);
	if (ret != 0)
	{
		destroy_simulation(simulation);
		return (print_err(ret));
	}
	destroy_philosophers(simulation->philosophers,
		simulation->number_of_philosophers);
	destroy_forks(simulation->forks, simulation->number_of_philosophers);
	destroy_simulation(simulation);
	return (SUCCESS);
}
