SRCS =	main.c \
		initialization.c \
		destruction.c \


all: $(SRCS)
	gcc -g -g3 -lpthread -Wall -Wextra $(SRCS) -o philo

fclean:
	rm philo

re: fclean all