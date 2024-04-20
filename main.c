/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erramos <erramos@student.42.rio>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 15:08:57 by erramos           #+#    #+#             */
/*   Updated: 2024/03/25 15:09:01 by erramos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/philo.h"


void	init_threads(t_table *infos)
{
	int	i;

	i = 0;
	while (i < infos->nb_philo)
	{
		pthread_create(&infos->philo->philo, NULL, filosofo, infos);
		infos->philo = infos->philo->next;
		i++;
		usleep(300);
	}
	i = 0;
	while (i < infos->nb_philo)
	{
		pthread_join(infos->philo->philo, NULL);
		infos->philo = infos->philo->next;
		i++;
	}
	pthread_join(infos->monitor, NULL);
}

int	long_time_without_eating(t_table *infos)
{
	t_philos	*temp;
	int			i;
	int			long_time;
	int			index;

	temp = infos->philo;
	i = 0;
	long_time = temp->last_time_eat;
	while (i < infos->nb_philo)
	{
		if (long_time < temp->last_time_eat)
		{
			long_time = temp->last_time_eat;
			index = temp->index;
		}
		i++;
		temp = temp->next;
	}
	return (index);
}

int all_philos_eat(t_table *infos)
{
	t_philos *philos;
	int		amount_philos_eat;

	philos = infos->philo;
	amount_philos_eat = 0;
	while (amount_philos_eat < (*infos).nb_philo)
	{
		if (philos->i_eat != infos->times_must_eat)
			return (0);
		philos = philos->next;
		amount_philos_eat++;
	}
	return (1);
}

void	*philos_monitoring(void *arg)
{
	t_philos	*philo;
	t_table		*infos;

	infos = (t_table *)arg;
	philo = infos->philo;
	while (1)
	{
		if (((get_real_time() - philo->last_time_eat) > infos->time_die)
			&& philo->status != 0 && philo->status != 2)
		{
			philo->status = 0;
			usleep(3500);
			printf("%ld %d died\n", (get_real_time() - infos->time_start)
				/ 1000, philo->index);
			infos->nb_philo--;
			pthread_mutex_unlock(&philo->fork);
			pthread_mutex_unlock(&philo->previous->fork);
		}
		if (infos->times_must_eat == philo->i_eat)
		{
			//printf("\nEu sou o filosofo %d eu comi %d vezes comforme a entrada %d\n", philo->index, philo->i_eat, infos->can_eat);
			philo->status = 0;
		}
		if (!infos->nb_philo)
			break ;
		if ((infos->flag_can_eat == 1) && all_philos_eat(infos))
			break ;
		philo = philo->next;
	}
	philo = philo->next;
	philo->status = 0;
	return (NULL);
}

int	main(int argc, char **argv)
{
	t_table	infos;

	if (!valid_args(argc - 1, argv + 1))
	{
		printf("Error\n");
		return (0);
	}
	init_infos(&infos, argv, argc);
	pthread_create(&infos.monitor, NULL, philos_monitoring, (void*)&infos);
	init_threads(&infos);
	usleep(3000);
	free_list(infos.philo);
	return (0);
}
