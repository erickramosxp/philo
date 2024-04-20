#include "../includes/philo.h"

int	verif_is_numeric(char *args)
{
	int	i;

	i = 0;
	while (args[i])
	{
		if (!ft_isdigit(args[i]))
			return (0);
		i++;
	}
	return (1);
}

int	valid_args(int argc, char **args)
{
	int	i;

	i = 0;
	if (argc < 4 || argc > 5)
		return (0);
	while (args[i])
	{
		if (!verif_is_numeric(args[i]))
			return (0);
		i++;
	}
	if (ft_atoi(args[0]) <= 0)
		return (0);
	return (1);
}