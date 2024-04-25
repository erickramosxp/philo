#include "../includes/philo.h"

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
		if (infos->nb_philo > 1 && philo->status)
		{
			printf("%ld %d has taken a fork\n", (get_real_time() - infos->time_start)
				/ 1000, philo->index);
			printf("%ld %d is eating\n", (get_real_time() - infos->time_start) / 1000,
				philo->index);
			philo->status = 2;
			new_sleep(infos->time_eat);
		//	usleep(infos->time_eat);
			philo->last_time_eat = get_real_time();
			philo->i_eat++;
			philo->status = 1;
			pthread_mutex_unlock(&philo->fork);
			pthread_mutex_unlock(&philo->previous->fork);
		}
	}
	// usleep(100);
}
void	philo_sleep(t_philos *philo, t_table *infos)
{
//	int	new_time;

	if (!philo->status)
		return ;
	printf("%ld %d is sleeping\n", (get_real_time() - infos->time_start) / 1000,
		philo->index);
	usleep(infos->time_sleep);
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
		/*
		if (philo->i_eat == infos->times_must_eat)
		{
			philo->status = 0;
			break;
		}*/
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