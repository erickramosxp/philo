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

long int	get_real_time(void)
{
	struct timeval	tv;
	long int		time;

	gettimeofday(&tv, NULL);
	time = (tv.tv_sec * 1000000) + tv.tv_usec;
	return (time);
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
}

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

void	free_list(t_philos *philo)
{
	t_philos	*temp;
	t_philos	*aux;
	int			i;

	i = 0;
	temp = philo->next;
	while (temp != philo)
	{
		aux = temp->next;
		printf("free no %d\n", temp->index);
		pthread_mutex_destroy(&temp->fork);
		free(temp);
		usleep(500);
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

void	philo_eat(t_philos *philo, t_table *infos)
{
	if (!philo->status)
		return ;
	usleep(100);
	if (philo->status)
	{
		pthread_mutex_lock(&philo->previous->fork);
		printf("%ld %d  has taken a fork\n", (get_real_time() - infos->time_start)
			/ 1000, philo->index);
		pthread_mutex_lock(&philo->fork);
		if (infos->nb_philo > 1)
		{
			printf("%ld %d has taken a fork\n", (get_real_time() - infos->time_start)
				/ 1000, philo->index);
			printf("%ld %d is eating\n", (get_real_time() - infos->time_start) / 1000,
				philo->index);
			philo->status = 2;
			new_sleep(infos->time_eat);
		//	usleep(infos->time_eat);
			philo->last_time_eat = get_real_time();
			pthread_mutex_unlock(&philo->fork);
			pthread_mutex_unlock(&philo->previous->fork);
			philo->status = 1;
		}
	}
	philo->i_eat++;
	// usleep(100);
}
void	philo_sleep(t_philos *philo, t_table *infos)
{
	int	new_time;

	if (!philo->status)
		return ;
	printf("%ld %d is sleeping\n", (get_real_time() - infos->time_start) / 1000,
		philo->index);
	usleep(infos->time_sleep);
	/*new_sleep(infos->time_sleep);
	new_time = get_real_time() + infos->time_sleep;
	while (get_real_time() < new_time)
		usleep(infos->time_sleep);*/
}

void	philo_think(t_philos *philo, t_table *infos)
{
	if (!philo->status)
		return ;
	printf("%ld %d is thinking\n", (get_real_time() - infos->time_start) / 1000,
		philo->index);
	usleep(infos->time_eat / 100);
}

void	*filosofo(void *arg)
{
	t_table		*infos;
	t_philos	*philo;

	infos = (t_table *)arg;
	philo = infos->philo;
	if (infos->philo->index % 2 == 0)
		usleep(3000);
	while (philo->status)
	{
		if (philo->i_eat == infos->times_must_eat)
		{
			philo->status = 0;
			break;
		}
		if (philo->status)
			philo_think(philo, infos);
		if (philo->status)
			philo_eat(philo, infos);
		if (philo->status)
			philo_sleep(philo, infos);
		usleep(100);
		if (!philo->status)
			break ;
	}
	return (NULL);
}

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
		//printf("\n\n%d\n\n", philo->index);
		if (((get_real_time() - philo->last_time_eat) > infos->time_die)
			&& philo->status != 0 && philo->status != 2)
		{
			philo->status = 0;
			printf("%ld %d died\n", (get_real_time() - infos->time_start)
				/ 1000, philo->index);
			infos->nb_philo--;
			usleep(500);
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
	infos.philo = start_philos(infos.nb_philo);
	index_philos(infos.philo, infos.nb_philo);
	pthread_create(&infos.monitor, NULL, philos_monitoring, &infos);
	init_threads(&infos);
	usleep(2000);
	free_list(infos.philo);
	return (0);
}
