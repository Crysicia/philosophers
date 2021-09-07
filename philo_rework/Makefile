CC			=	gcc
CFLAGS 		=	-Wall -Wextra -Werror
RM 			=	rm -f
NAME 		=	philo
LIBS 		=	-lpthread
SRCS 		=	main.c \
				parsing.c \
				simulation.c
OBJS 		= $(SRCS:.c=.o)

all: $(NAME)

.c.o:
	$(CC) $(CFLAGS) $(HEADERS) -c $< -o $@

$(NAME): $(OBJS)
	$(CC) $(OBJS) $(LIBS) -o $(NAME)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
