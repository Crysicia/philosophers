#include "header.h"

int ft_atoi_strict(const char *str)
{
	int result;

	result = 0;
	while (*str)
	{
		if (*str < '0' || *str > '9')
			return (-1);
		result = (result * 10) + (*str - '0');
		if (result < 0)
			return (-2);
		str++;
	}
	return (result);
}

t_simulation *parse_arguments(int argc, char *argv[])
{
	t_simulation *simulation;

	if (argc != 5)
		return (NULL);
	simulation = malloc(sizeof(t_simulation));
	if (!simulation)
		return (NULL);
	simulation->number_of_philosophers = ft_atoi_strict(argv[NUMBER_OF_PHILOSOPHERS]);
	simulation->time_to_die = ft_atoi_strict(argv[TIME_TO_DIE]);
	simulation->time_to_eat = ft_atoi_strict(argv[TIME_TO_EAT]);
	simulation->time_to_sleep = ft_atoi_strict(argv[TIME_TO_SLEEP]);
	// simulation->number_of_meals = ft_atoi_strict(argv[NUMBER_OF_MEALS]);
	if (simulation->number_of_philosophers == -1 || simulation->time_to_die == -1 || simulation->time_to_eat == -1 || simulation->time_to_sleep == -1)
	{
		free(simulation);
		return (NULL);
	}
	return (simulation);
}
