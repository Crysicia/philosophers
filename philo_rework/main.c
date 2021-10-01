#include "header.h"

void print_err(unsigned int code)
{
	static char *errors[NUMBER_OF_ERRORS] = { MSG_COULD_NOT_INITIALIZE_PHILOS, MSG_COULD_NOT_INITIALIZE_FORKS, MSG_COULD_NOT_CREATE_THREAD };
	char *message;

	message = MSG_UNEXPECTED_ERR;
	if (code <= NUMBER_OF_ERRORS)
		message = errors[code - 1];
	dprintf(STDERR_FILENO, "ERROR: %s\n", message);
}

void destroy_simulation(t_simulation *simulation)
{
	destroy_philosophers(simulation->philosophers, simulation->number_of_philosophers);
	destroy_forks(simulation->forks, simulation->number_of_philosophers);
	destroy_lock(simulation->write_lock);
	destroy_lock(simulation->access_lock);
	free(simulation);
}

int main(int argc, char *argv[])
{
	t_simulation *simulation;

	simulation = parse_arguments(argc, argv);
	if (!simulation)
	{
		printf("ERROR: Unexpected error when parsing arguments\n");
		return (1);
	}
	unsigned int ret = launch_simulation(simulation);
	if (ret != 0)
		print_err(ret);
	destroy_simulation(simulation);
	return (ret);
}
