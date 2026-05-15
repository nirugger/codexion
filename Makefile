NAME	= codexion

CC		= cc
CFLAGS	= -Wall -Wextra -Werror -pthread

SRCS	= main.c \
		  coder_routine.c \
		  monitor.c \
		  queue.c \
		  codexion.c \
		  codexion_init.c \
		  parser.c \
		  utils.c

OBJS	= $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

%.o: %.c codexion.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

N_CODERS	= 5
BURNOUT		= 1000
COMPILE		= 175
DEBUG		= 130
REFACTOR	= 125
N_COMPILES	= 5
COOLDOWN	= 70
SCHEDULER	= edf
 
run: $(NAME)
	./$(NAME) $(N_CODERS) $(BURNOUT) $(COMPILE) $(DEBUG) $(REFACTOR) $(N_COMPILES) $(COOLDOWN) $(SCHEDULER)

.PHONY: all clean fclean re run