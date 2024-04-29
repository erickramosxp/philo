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
		infos->philo = next_philo(infos->philo, &infos->table_mutex);
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

int	all_philos_eat(t_table *infos)
{
	t_philos	*philos;
	int			amount_philos_eat;

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
/*
void	disable_all_philos(t_table *infos, mutex_p *mutex)
{
	int			i;
	t_philos	*philos;

	philos = infos->philo;
	i = 0;
	while (i < infos->nb_philo)
	{
		pthread_mutex_lock(mutex);
		philos->status = 0;
		pthread_mutex_unlock(mutex);
		philos = philos->next;
		i++;
	}
}*/
/*
void	set_status(t_philos *philo, mutex_p *mutex, int philo_status)
{
	pthread_mutex_lock(mutex);
	philo->status = philo_status;
	pthread_mutex_unlock(mutex);
}*/

void	verif_philo_is_dead(t_table *infos, t_philos *philo)
{
	pthread_mutex_lock(&philo->fork);
	if (((get_real_time() - philo->last_time_eat) > infos->time_die)
		&& philo->status != 0 && philo->status != 2)
	{
		set_status(&philo->status, 0, &philo->set_status);
	//	philo->status = 0;
		print_status("died\n", (get_real_time() - infos->time_start) / 1000, philo->index, &(infos->print_mutex));
		
	/* printf("%ld %d died\n", (get_real_time() - infos->time_start) / 1000,
			philo->index);*/
		infos->philos_dead--;
	}
	pthread_mutex_unlock(&philo->fork);
}

void	disable_all_philos(t_table *infos, t_philos *philos)
{
	int	i;
	t_philos *temp;

	i = 0;
	temp = philos;
	while (i <= infos->nb_philo)
	{
		set_status(&temp->status, 0, &temp->set_status);
		temp = temp->next;
		i++;
	}
}

void	*philos_monitoring(void *arg)
{
	t_philos	*philo;
	t_table		*infos;

	infos = (t_table *)arg;
	philo = set_philo(infos, &infos->table_mutex);
//	philo = infos->philo;
	while (1)
	{
		verif_philo_is_dead(infos, philo);
		pthread_mutex_lock(&infos->table_mutex);
		if (infos->times_must_eat == philo->i_eat)
		{
			philo->status = 0;
		}
		pthread_mutex_unlock(&infos->table_mutex);
		pthread_mutex_lock(&infos->table_mutex);
		if (!infos->flag_can_eat && (infos->nb_philo != infos->philos_dead))
		{
			disable_all_philos(infos, philo);
			break ;
		}
		pthread_mutex_unlock(&infos->table_mutex);
		if ((infos->flag_can_eat == 1) && all_philos_eat(infos))
			break ;
		philo = philo->next;
	}
	philo = philo->next;
	philo->status = 0;
	return (NULL);
}

t_philos *set_philo(t_table *infos, mutex_p *table)
{
	t_philos *temp;
	pthread_mutex_lock(table);
	temp = infos->philo;
	pthread_mutex_unlock(table);
	return (temp);
}

t_philos *next_philo(t_philos *philo, mutex_p *table)
{
	t_philos *temp;
	pthread_mutex_lock(table);
	temp = philo->next;
	pthread_mutex_unlock(table);
	return (temp);
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
	pthread_create(&infos.monitor, NULL, philos_monitoring, (void *)&infos);
	init_threads(&infos);
	usleep(3000);
	free_list(infos.philo);
	return (0);
}
