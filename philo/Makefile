SRCS =	main.c \
		initialization.c \
		destruction.c \
		time.c \


all: $(SRCS)
	gcc -g -g3 -lpthread -Wall -Wextra -fsanitize=thread $(SRCS) -o philo

fclean:
	rm philo

re: fclean all
# -fsanitize=thread
