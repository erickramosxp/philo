

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <sys/time.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

typedef struct s_philo
{
	int				index;
	pthread_t		philo;
	pthread_mutex_t	fork;
	struct s_philo	*previous;
	struct s_philo	*next;
}					t_philos;

typedef struct s_table
{
	int			nb_philo;
	long int	time_die;
	long int	time_eat;
	long int	time_sleep;
	int			times_must_eat;
	long int	time_start;
	t_philos	*philo;
}					t_table;

int					ft_atoi(const char *nptr);
long				ft_atol(const char *nptr);
int					ft_isdigit(int c);

#endif