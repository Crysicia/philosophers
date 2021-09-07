#include "header.h"

int main(int argc, char *argv[])
{
	t_simulation *simulation;

	simulation = parse_arguments(argc, argv);
	if (!simulation)
	{
		printf("ERROR: Unexpected error when parsing arguments\n");
		return (1);
	}
	launch_simulation(simulation);
}
