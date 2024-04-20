NAME = philo
SRCS = utils.c check_args.c routine.c start_philos.c
FLAGS = -Wall -Wextra -Werror
SANITIZE = -fsanitize=thread

NAME: all

all:
	cc $(FLAGS) $(addprefix src/, $(SRCS)) main.c -o philo -pthread
clean:
		rm philo

fclean:

re: clean NAME