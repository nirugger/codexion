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

STDRUN	= $(NAME) $(N_CODERS) $(BURNOUT) $(COMPILE) $(DEBUG) $(REFACTOR) $(N_COMPILES) $(COOLDOWN) $(SCHEDULER)
LNTRUN	= $(NAME) 3 2000 200 200 200 3 150 fifo
FTSRUN	= $(NAME) 42 7492 420 372 291 4 226 fifo --visual


CC			= cc
CFLAGS		= -Wall -Wextra -Werror -pthread

TRUEFALSE	= && echo "$(GREEN)OK!$(RESET)" || echo "$(RED)KO!$(RESET)"
NL			= echo ""

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
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
	@$(NL)
	@echo "$(GREEN)codexion made ✓$(RESET)"

%.o: %.c codexion.h
	@$(CC) $(CFLAGS) -c $< -o $@


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
	@$(NL)
	@echo "$(WHITE)entering the codexion...$(RESET)"
	@./$(STDRUN) \
	$(TRUEFALSE)
	@$(NL)
	@rm -f $(OBJS)
	@echo "$(GREEN)clean made ✓$(RESET)"

lint:
	@$(NL)
	@echo "$(YELLOW)norminetting...$(RESET)"
	@$(NORM) \
	$(TRUEFALSE)
	@$(NL)
	@sleep 1

val: $(NAME)
	@$(NL)
	@echo "$(MAGENTA)valgrinding...$(RESET)"
	@$(VAL) $(VALFLAGS) ./$(LNTRUN)
	@echo "$(MAGENTA)valgrinded ✓$(RESET)"
	@$(NL)
	@sleep 1

hel: $(NAME)
	@$(NL)
	@echo "$(RED)helgrinding...$(RESET)"
	@$(VAL) $(HEL) $(HELFLAGS) ./$(LNTRUN)
	@echo "$(RED)helgrinded ✓$(RESET)"
	@$(NL)
	@sleep 1

lint-strict: lint val hel

run--visual: $(NAME)
	clear
	@echo "$(BLUE)entering the codexion...$(RESET)"
	@./$(STDRUN) $(VISUAL)\
	$(TRUEFALSE)
	@$(NL)
	@rm -f $(OBJS)
	@echo "$(GREEN)clean made ✓$(RESET)"

frun: fclean all lint-strict
	@$(NL)
	@echo "$(BLUE)entering the codexion...$(RESET)"
	@sleep 2
	@echo "$(MAGENTA)the codexion begins!$(RESET)"
	@$(NL)
	@sleep 1
	@./$(FTSRUN) \
	$(TRUEFALSE)
	@$(NL)
	@rm -f $(OBJS)
	@rm -f $(NAME)
	@echo "$(GREEN)fclean made ✓✓✓$(RESET)"

.PHONY: all clean fclean re run lint val hel lint-strict run--visual frun