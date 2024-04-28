#include "../includes/philo.h"

void	philo_eat(t_philos *philo, t_table *infos)
{
	if (!philo->status)
		return ;
	usleep(100);
	if (philo->status)
	{
		pthread_mutex_lock(&philo->previous->fork);
		print_status("has taken a fork\n", (get_real_time() - infos->time_start) / 1000, philo->index, &infos->print_mutex);
		pthread_mutex_lock(&philo->fork);

		print_status("has taken a fork\n", (get_real_time() - infos->time_start) / 1000, philo->index, &infos->print_mutex);
		print_status("is eating\n", (get_real_time() - infos->time_start) / 1000, philo->index, &infos->print_mutex);
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
void	philo_sleep(t_philos *philo, t_table *infos)
{
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

int		philo_is_alive(t_philos *philo, mutex_p *status_mutex)
{
	pthread_mutex_lock(status_mutex);
	if (philo->status == 0)
	{
		pthread_mutex_unlock(status_mutex);
		return (0);
	}
	return (1);
}

void	*filosofo(void *arg)
{
	t_table		*infos;
	t_philos	*philo;

	infos = (t_table *)arg;
	philo = infos->philo;
	if (infos->philo->index % 2 == 0)
		usleep(3000);
	while (1)
	{
		if (!philo_is_alive(philo, &infos->status_mutex))
			break ;
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