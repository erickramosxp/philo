#include "../includes/philo.h"

void	philo_eat(t_philos *philo, t_table *infos)
{
	usleep(100);
	pthread_mutex_lock(&philo->previous->fork);
	print_status("has taken a fork\n", (get_real_time() - infos->time_start) / 1000, philo->index, &(infos->print_mutex));
	if (infos->nb_philo == infos->philos_dead && philo->status)
	{
		pthread_mutex_lock(&philo->fork);
		print_status("has taken a fork\n", (get_real_time() - infos->time_start) / 1000, philo->index, &(infos->print_mutex));
		print_status("is eating\n", (get_real_time() - infos->time_start) / 1000, philo->index, &(infos->print_mutex));
	//	set_status(&philo->status, 2, &philo->set_status);
		new_sleep(infos->time_eat);
		philo->last_time_eat = get_real_time();
		pthread_mutex_lock(&infos->table_mutex);
		philo->i_eat++;
		pthread_mutex_unlock(&infos->table_mutex);
	//	set_status(&philo->status, 1, &philo->set_status);
		pthread_mutex_unlock(&philo->fork);
		pthread_mutex_unlock(&philo->previous->fork);
	}
}
void	philo_sleep(t_philos *philo, t_table *infos)
{
	print_status("is sleeping\n", (get_real_time() - infos->time_start) / 1000, philo->index, &(infos->print_mutex));
	usleep(infos->time_sleep);
}

void	philo_think(t_philos *philo, t_table *infos)
{
	print_status("is thinking\n", (get_real_time() - infos->time_start) / 1000, philo->index, &(infos->print_mutex));
}

void	*filosofo(void *arg)
{
	t_table		*infos;
	t_philos	*philo;

	infos = (t_table *)arg;
	philo = set_philo(infos, &infos->table_mutex);
	if (philo->index % 2 == 0)
		usleep(3000);
	while (1)
	{
		philo_think(philo, infos);
		philo_eat(philo, infos);
		philo_sleep(philo, infos);
	//	pthread_mutex_lock(&philo->set_status);
		if (philo->status == 0){
	//		pthread_mutex_unlock(&philo->set_status);
			break ;
		}
		//usleep(100);
	}
	return (NULL);
}