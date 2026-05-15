NAME	= codexion

N_CODERS	= 5
BURNOUT		= 1200
COMPILE		= 200
DEBUG		= 200
REFACTOR	= 200
N_COMPILES	= 5
COOLDOWN	= 100
SCHEDULER	= fifo
TVANNI	= $(NAME) $(N_CODERS) $(BURNOUT) $(COMPILE) $(DEBUG) $(REFACTOR) $(N_COMPILES) $(COOLDOWN) $(SCHEDULER)

CC		= cc
VG		= valgrind
HG		= --tool=helgrind
VF		= --leak-check=full
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
 
run: $(NAME)
	./$(TVANNI)

valgrind: $(NAME)
	$(VG) $(VF) ./$(TVANNI)

helgrind: $(NAME)
	$(VG) $(HG) ./$(TVANNI)

.PHONY: all clean fclean re run valgrind helgrind