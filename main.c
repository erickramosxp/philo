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
//	free(tv);
	return (time);
}

void	init_infos(t_table *infos, char **args)
{
	infos->nb_philo = ft_atoi(args[1]);
	infos->time_die = ft_atoi(args[2]) * 1000;
	infos->time_eat = ft_atoi(args[3]) * 1000;
	infos->time_sleep = ft_atoi(args[4]) * 1000;
	infos->time_start = get_real_time();
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
		philo->status = 0;
		philo->last_time_eat = get_real_time();
		pthread_mutex_init(&philo->fork, NULL);
		philo = philo->next;
		i++;
	}
}

void	free_list(t_philos *philo)
{
	t_philos *temp;
	t_philos *aux;

	temp = philo->next;
	while (temp != philo)
	{
		aux = temp->next;
		free(temp);
		temp = aux;
	}
	free(philo);
}

void	*filosofo(void *arg)
{
    // Código do filósofo
	t_table *infos;

	infos = (t_table *)arg;
	if (((get_real_time() - infos->philo->last_time_eat) > infos->time_die) && infos->philo->status != 1)
	{
		infos->philo->status = 1;
		printf("%ld Filósofo %d esta morto.\n", (get_real_time() - infos->time_start) / 1000, infos->philo->index);
		infos->nb_philo--;
	}
	if (infos->philo->status == 0)
	{
		pthread_mutex_lock(&infos->philo->fork);
		pthread_mutex_lock(&infos->philo->previous->fork);
		printf("%ld Filósofo %d pegou o garfo.\n", (get_real_time() - infos->time_start) / 1000, infos->philo->index);
		printf("%ld Filósofo %d está comendo.\n", (get_real_time() - infos->time_start) / 1000, infos->philo->index);
		usleep(infos->time_eat);
		pthread_mutex_unlock(&infos->philo->fork);
		pthread_mutex_unlock(&infos->philo->previous->fork);
		infos->philo->last_time_eat = get_real_time();
		infos->philo->status = 2;
	}
	else if (infos->philo->status == 2)
	{
		printf("%ld Filósofo %d está dormindo.\n", (get_real_time() - infos->time_start) / 1000, infos->philo->index);
		usleep(infos->time_sleep);
		infos->philo->status = 3;
	}
	else if (infos->philo->status == 3)
	{
		printf("%ld Filósofo %d está pensando.\n", (get_real_time() - infos->time_start) / 1000, infos->philo->index);
		infos->philo->status = 0;
	}
//	usleep(30000);
    return NULL;
}

int main(int argc, char **argv)
{
	t_table infos;

	if (!valid_args(argc - 1, argv + 1))
	{
		printf("Error\n");
		return (0);
	}
	init_infos(&infos, argv);
	infos.philo = start_philos(infos.nb_philo);
	index_philos(infos.philo, infos.nb_philo);
	int i;

	i = 0;
	while (infos.nb_philo)
	{
		pthread_create(&infos.philo->philo, NULL, filosofo, &infos);
	//	pthread_detach(infos.philo->philo);
		pthread_join(infos.philo->philo, NULL);
		infos.philo = infos.philo->next;
	}
/*	i = 0;
	while (i < infos.nb_philo) {
		infos.philo = infos.philo->next;
		i++;
    }*/
//	printf("%ld\n%ld\n%ld\n%ld", infos.time_start, infos.time_die, infos.time_eat, infos.time_sleep);
/*	while (1)
	{
		printf("%d\n", infos.philo->index);
		infos.philo = infos.philo->next;
		usleep(1000000);
	}*/
	free_list(infos.philo);
	return (0);
}
