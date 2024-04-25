

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <sys/time.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

typedef enum status{
	DIE,
	LIVE,
	EAT,
	SLEEP,
	THINK
} status;

typedef pthread_mutex_t mutex_p;

typedef struct s_philo
{
	int				index;
	int				can_eat;
	int				status;
	pthread_t		philo;
	mutex_p			fork;
	long int		last_time_eat;
	int				i_eat;
	int				get_fork_right;
	int				get_fork_left;
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
	int			can_eat;
	int			flag_can_eat;
	int			philos_dead;
	long int	time_start;
	mutex_p		table_mutex;
	pthread_t	monitor;
	t_philos	*philo;
}					t_table;

int					ft_atoi(const char *nptr);
long				ft_atol(const char *nptr);
int					ft_isdigit(int c);

void				init_infos(t_table *infos, char **args, int argc);
void				*filosofo(void *arg);
long int			get_real_time(void);
int					valid_args(int argc, char **args);
void				free_list(t_philos *philo);

void				new_sleep(long time);

#endif