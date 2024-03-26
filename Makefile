NAME = philo
SRCS = utils.c
FLAGS = -Wall -Wextra -Werror

NAME: all

all:
	cc $(FLAGS) $(SRCS) main.c
clean:

fclean:

re: