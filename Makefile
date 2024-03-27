NAME = philo
SRCS = utils.c
FLAGS = -Wall -Wextra -Werror

NAME: all

all:
	cc $(FLAGS) $(addprefix src/, $(SRCS)) main.c
clean:

fclean:

re: