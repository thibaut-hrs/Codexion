CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread -fsanitize=address
SRCS =	src/main.c \
		src/parsing.c \
		src/init_simulation.c \
		src/coder.c \
		src/monitor.c \
		src/scheduler.c \
		src/heap_operations.c \
		src/queue_operations.c \
		src/utils.c
OBJS = $(SRCS:.c=.o)
NAME = codexion

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

$(OBJS): %.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
