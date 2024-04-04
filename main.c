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
		philo->status = 1;
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

	int i = 0;
/*	while (i < 5)
	{
		pthread_join(philo->philo, NULL);
		philo = philo->next;
		i++;
	}*/
	temp = philo->next;
	while (temp != philo)
	{
		aux = temp->next;
		printf("free no %d\n", temp->index);
	//	pthread_mutex_destroy(&temp->fork);
		free(temp);
		usleep(500);
		temp = aux;
	}
	printf("free no %d\n", philo->index);
//	pthread_mutex_destroy(&philo->fork);
	free(philo);
}

void philo_eat(t_table *infos)
{
	if (infos->philo->index % 2 == 0)
	{
		pthread_mutex_lock(&infos->philo->fork);
		printf("%ld Filósofo %d pegou o garfo da direita um.\n", (get_real_time() - infos->time_start) / 1000, infos->philo->index);
	}
	else
	{
		pthread_mutex_lock(&infos->philo->previous->fork);
		printf("%ld Filósofo %d pegou o garfo da esquerda um.\n", (get_real_time() - infos->time_start) / 1000, infos->philo->index);
	}
	

	if (infos->philo->index % 2 == 0)
	{
		pthread_mutex_lock(&infos->philo->previous->fork);
		printf("%ld Filósofo %d pegou o garfo da esquerda dois.\n", (get_real_time() - infos->time_start) / 1000, infos->philo->index);
	}
	else
	{
		pthread_mutex_lock(&infos->philo->fork);
		printf("%ld Filósofo %d pegou o garfo da direita dois.\n", (get_real_time() - infos->time_start) / 1000, infos->philo->index);
	}

	printf("%ld Filósofo %d está comendo.\n", (get_real_time() - infos->time_start) / 1000, infos->philo->index);
	usleep(infos->time_eat);
	infos->philo->last_time_eat = get_real_time();
	pthread_mutex_unlock(&infos->philo->fork);
	pthread_mutex_unlock(&infos->philo->previous->fork);
	//usleep(100);
}
void	philo_sleep(t_table *infos)
{
	printf("%ld Filósofo %d está dormindo.\n", (get_real_time() - infos->time_start) / 1000, infos->philo->index);
	usleep(infos->time_sleep);
}

void	philo_think(t_table *infos)
{
	printf("%ld Filósofo %d está pensando.\n", (get_real_time() - infos->time_start) / 1000, infos->philo->index);
	usleep(50);
}


void	*filosofo(void *arg)
{
    // Código do filósofo
	t_table *infos;
	t_philos *philo;

	infos = (t_table *)arg;
	philo = infos->philo;
	if (!infos->philo->index % 2)
		usleep(2000);
	while (philo->status)
	{
		if (((get_real_time() - philo->last_time_eat) > infos->time_die) && philo->status != 0)
		{
			philo->status = 0;
			printf("%ld Filósofo %d esta morto.\n", (get_real_time() - infos->time_start) / 1000, philo->index);
			infos->nb_philo--;
		}
		philo_think(infos);
		philo_eat(infos);
		philo_sleep(infos);
	}
/*	if (philo->status == 0)
	{
		philo_eat(infos);
	}
	else if (philo->status == 2)
	{
		printf("%ld Filósofo %d está dormindo.\n", (get_real_time() - infos->time_start) / 1000, philo->index);
		usleep(infos->time_sleep);
		philo->status = 3;
	}
	else if (philo->status == 3)
	{
		printf("%ld Filósofo %d está pensando.\n", (get_real_time() - infos->time_start) / 1000, philo->index);
		philo->status = 0;
	}*/
//	usleep(30000);
    return NULL;
}

void	init_threads(t_table *infos)
{
	int i;

	i = 0;
	while (i < infos->nb_philo)
	{
		pthread_create(&infos->philo->philo, NULL, filosofo, infos);
	//	pthread_detach(infos->philo->philo);
		usleep(50);
	//	usleep(10000);
	//	pthread_join(infos->philo->philo, NULL);
		infos->philo = infos->philo->next;
		i++;
	}
	i = 0;
	while (i < infos->nb_philo)
	{
		pthread_join(infos->philo->philo, NULL);
		infos->philo = infos->philo->next;
		i++;
	}
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
	init_threads(&infos);
	/*while (1)
	{}*/
	free_list(infos.philo);
	return (0);
}
