NAME	= codexion

N_CODERS	= 5
BURNOUT		= 1200
COMPILE		= 200
DEBUG		= 200
REFACTOR	= 200
N_COMPILES	= 5
COOLDOWN	= 100
SCHEDULER	= fifo
VISUAL		= --visual

RUN		= $(NAME) $(N_CODERS) $(BURNOUT) $(COMPILE) $(DEBUG) $(REFACTOR) $(N_COMPILES) $(COOLDOWN) $(SCHEDULER)
LINTRUN	= $(NAME) 3 2000 200 200 200 3 150 fifo
FULLRUN	= $(NAME) 42 7492 420 372 291 5 226 fifo --visual


CC			= cc
CFLAGS		= -Wall -Wextra -Werror -pthread
TRUEFALSE	= && echo "$(GREEN)OK!$(RESET)" || echo "$(RED)KO!$(RESET)"

NORM		= norminette
NORMFLAGS	= >/dev/null 2>&1
VAL			= valgrind
HEL			= --tool=helgrind
VALFLAGS	= --leak-check=full
HELFLAGS	= --free-is-write=yes

RED		= \033[0;31m
GREEN	= \033[0;32m
YELLOW	= \033[0;33m
BLUE	= \033[0;34m
MAGENTA	= \033[0;35m
CYAN	= \033[0;36m
WHITE	= \033[0;37m
RESET	= \033[0m

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
	@rm -f $(OBJS)
	@echo "$(GREEN)clean made ✓$(RESET)"

fclean:
	clear
	@rm -f $(OBJS)
	@rm -f $(NAME)
	@echo "$(GREEN)fclean made ✓✓✓$(RESET)"

re: fclean all

run: $(NAME)
	clear
	@echo "$(WHITE)entering the Codexion...$(RESET)"
	@./$(RUN) \
	$(TRUEFALSE)

lint:
	clear
	@echo "$(YELLOW)norminetting...$(RESET)"
	@$(NORM) \
	$(TRUEFALSE)

val: $(NAME)
	@echo "$(MAGENTA)valgrinding...$(RESET)"
	@$(VAL) $(VALFLAGS) ./$(LINTRUN)
	@echo "$(MAGENTA)valgrinded ✓$(RESET)"

hel: $(NAME)
	@echo "$(RED)helgrinding...$(RESET)"
	@$(VAL) $(HEL) $(HELFLAGS) ./$(LINTRUN)
	@echo "$(RED)helgrinded ✓$(RESET)"

lint-strict: lint val hel

run--visual: $(NAME)
	clear
	@echo "$(BLUE)entering the Codexion...$(RESET)"
	@./$(RUN) $(VISUAL) \
	$(TRUEFALSE)

frun: fclean all lint-strict
	clear
	@echo "$(BLUE)entering the Codexion...$(RESET)"
	@sleep 2
	clear
	@echo "$(MAGENTA)the Codexion begins!$(RESET)"
	@./$(FULLRUN) \
	$(TRUEFALSE)


.PHONY: all clean fclean re run lint val hel lint-strict run--visual frun