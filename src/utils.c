#include "../includes/philo.h"

int	ft_atoi(const char *nptr)
{
	int	resp;
	int	signal;
	int	i;

	i = 0;
	resp = 0;
	signal = 1;
	while (nptr[i] == ' ' || (nptr[i] >= 9 && nptr[i] <= 13))
		i++;
	if (nptr[i] == '+' || nptr[i] == '-')
	{
		if (nptr[i] == '-')
			signal = signal *(-1);
		i++;
	}
	while (nptr[i] >= 48 && nptr[i] <= 57)
	{
		resp = resp * 10 + (nptr[i] - '0');
		i++;
	}
	return (resp * signal);
}

long	ft_atol(const char *nptr)
{
	long	resp;
	long	signal;
	int		i;

	i = 0;
	resp = 0;
	signal = 1;
	while (nptr[i] == ' ' || (nptr[i] >= 9 && nptr[i] <= 13))
		i++;
	if (nptr[i] == '+' || nptr[i] == '-')
	{
		if (nptr[i] == '-')
			signal = signal *(-1);
		i++;
	}
	while (nptr[i] >= 48 && nptr[i] <= 57)
	{
		resp = resp * 10 + (nptr[i] - '0');
		i++;
	}
	return (resp * signal);
}

int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	else
		return (0);
}

long int	get_real_time(void)
{
	struct timeval	tv;
	long int		time;

	gettimeofday(&tv, NULL);
	time = (tv.tv_sec * 1000000) + tv.tv_usec;
	return (time);
}

void	free_list(t_philos *philo)
{
	t_philos	*temp;
	t_philos	*aux;

	temp = philo->next;
	while (temp != philo)
	{
		usleep(500);
		aux = temp->next;
		printf("free no %d\n", temp->index);
		if (pthread_mutex_destroy(&temp->fork))
			printf("Eu filosofo %d não fui destruido.\n", aux->index);
		free(temp);
		temp = aux;
	}
	printf("free no %d\n", philo->index);
	free(philo);
}

void	new_sleep(long time)
{
	long new_time;

	new_time = get_real_time() + time;
	while (get_real_time() < new_time)
	{
		usleep(100);
	}
}