NAME = philo
SRCS = ft_atoi.c
FLAGS = -Wall -Wextra -Werror

NAME: all

all:
	cc $(FLAGS) $(SRCS) main.c
clean:

fclean:

re: