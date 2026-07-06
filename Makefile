NAME = ft_ping
CC = cc
CFLAGS = -g -Wall -Wextra -Werror 
SRC = src/main.c \
      src/ping.c \
      src/utils.c \
      src/packet.c \
      src/icmp.c \
      src/parse.c 

OBJS = $(SRC:.c=.o)

all: $(NAME)


$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) -lm

clean: 
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
