#include "../includes/philo.h"

t_philos	*start_philos(int qtd_philos)
{
	t_philos	*philo;
	t_philos	*new_philo;
	t_philos	*prev;
	t_philos	*head;

	prev = NULL;
	new_philo = malloc(sizeof(t_philos));
	if (!new_philo)
		return (NULL);
	head = new_philo;
	while (qtd_philos)
	{
		philo = new_philo;
		philo->previous = prev;
		if (prev)
			prev->next = philo;
		prev = philo;
		qtd_philos--;
		if (qtd_philos)
		{
			new_philo = malloc(sizeof(t_philos));
			philo = philo->next;
		}
	}
	philo->next = head;
	head->previous = philo;
	return (head);
}

void	index_philos(t_philos *philo, int qtd_philos)
{
	int	i;

	i = 1;
	while (i <= qtd_philos)
	{
		philo->index = i;
		philo->status = 1;
		philo->can_eat = 0;
		philo->i_eat = 0;
		philo->last_time_eat = get_real_time();
		pthread_mutex_init(&philo->fork, NULL);
		philo = philo->next;
		i++;
	}
}

void	init_infos(t_table *infos, char **args, int argc)
{
	infos->nb_philo = ft_atoi(args[1]);
	infos->time_die = ft_atoi(args[2]) * 1000;
	infos->time_eat = ft_atoi(args[3]) * 1000;
	infos->time_sleep = ft_atoi(args[4]) * 1000;
	infos->time_start = get_real_time();
	if (argc == 6)
	{
		infos->times_must_eat = ft_atoi(args[5]);
		infos->flag_can_eat = 1;
	}
	else
	{
		infos->times_must_eat = -1;
		infos->flag_can_eat = 0;
	}
	infos->philo = NULL;
    infos->philo = start_philos(infos->nb_philo);
    index_philos(infos->philo, infos->nb_philo);
	pthread_mutex_init(&infos->table_mutex, NULL);
}