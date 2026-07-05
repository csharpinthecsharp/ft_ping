NAME = ft_ping
CC = cc
CFLAGS = -g 
SRC = main.c \
      ping.c \
      utils.c \
      packet.c \
      icmp.c \
      parse.c 

OBJS = $(SRC:.c=.o)

all: $(NAME)


$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

clean: 
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
