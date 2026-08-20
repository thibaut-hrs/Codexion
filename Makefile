CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread
SRCS =	src/main.c \
		src/parsing_utils.c \
		src/parsing.c
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