CC			=	gcc
CFLAGS 		=	-Wall -Wextra -Werror -fsanitize=thread -g
RM 			=	rm -f
NAME 		=	philo
LIBS 		=	-lpthread
SRCS 		=	main.c \
				parsing.c \
				simulation.c \
				init.c \
				philo_actions.c \
				time.c \
				watcher.c \
				access.c
OBJS 		= $(SRCS:.c=.o)

all: $(NAME)

.c.o:
	$(CC) $(CFLAGS) $(HEADERS) -c $< -o $@

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $(NAME)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
