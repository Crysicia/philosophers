#include "header.h"

void print_err(unsigned int code)
{
	static char *errors[NUMBER_OF_ERRORS] = { MSG_COULD_NOT_INITIALIZE_PHILOS, MSG_COULD_NOT_INITIALIZE_FORKS, MSG_COULD_NOT_CREATE_THREAD };

	dprintf(STDERR_FILENO, "ERROR: %s\n", errors[code - 1]);
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
	return (ret);
}
