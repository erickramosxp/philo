

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <sys/time.h>
# include <stdbool.h>

typedef struct s_philo
{
	int				index;
	pthread_t		philo;
	pthread_mutex_t	fork;
	bool			sleep;
	struct philo	*next;
}					t_philos;

int					ft_atoi(const char *nptr);
int					ft_isdigit(int c);

#endif