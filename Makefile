NAME	= codexion


NB_CODERS	= 300
BURNOUT		= 120000
COMPILE		= 200
DEBUG		= 200
REFACTOR	= 200
NB_COMPILES	= 3
COOLDOWN	= 100
SCHEDULER	= fifo
VISUAL		= --visual

ST	= $(NB_CODERS) $(BURNOUT) $(COMPILE) $(DEBUG) $(REFACTOR) $(NB_COMPILES) $(COOLDOWN) $(SCHEDULER)
CR	= $(NB_CODERS) $(BURNOUT) $(COMPILE) $(DEBUG) $(REFACTOR) $(NB_COMPILES) $(COOLDOWN) $(SCHEDULER)
LT	=  3 2000 200 200 200 3 150 fifo
FT	= 42 9492 420 372 291 4 226 edf --visual

CC			= cc
CFLAGS		= -Wall -Wextra -Werror -pthread

TRUEFALSE	= && echo "$(GREEN)OK!$(RESET)" || echo "$(RED)KO!$(RESET)"
NEWLINE		= echo ""

NORM		= norminette
NORMFLAG	= >/dev/null 2>&1
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

# --- codexion make commands -------------------------------------------------
this:
	clear
	@printf "\n"
	@printf "    master the Makefile before the Makefile masters you\n"
	@printf "\n"
	@printf "    ╔                                                 ╗\n"
	@printf "    ║  st :  $(ST)            ║\n"
	@printf "    ║  lt :  $(LT)            ║\n"
	@printf "    ║  ft : $(FT)   ║\n"
	@printf "    ╚                                                 ╝\n"
	@printf "\n"
	@printf "      make            this\n"
	@printf "  ═════════════════════════════════════════════════════════════════════\n"
	@printf "      codexion        compile *.c -Wall -Werror -Wextra -pthread\n"
	@printf "      all             compile *.c preserving objects\n"
	@printf "      clean           remove binary\n"
	@printf "      fclean          remove binary and objects\n"
	@printf "      re              clean + codexion\n"
	@printf "  ═════════════════════════════════════════════════════════════════════\n"
	@printf "      run             execute codexion                   [st]\n"
	@printf "      run--visual     execute colorexion                 [st --visual]\n"
	@printf "      ft_run          run--visual + clean                [ft]\n"
	@printf "      frun            re + flint + run--visual + clean   [st]\n"
	@printf "  ═════════════════════════════════════════════════════════════════════\n"
	@printf "      lint            norminette\n"
	@printf "      val             valgrind --leak-check=full         [lt]\n"
	@printf "      hel             valgrind --tool=helgrind           [lt]\n"
	@printf "      flint           lint + val + hel                   [lt]\n"
	@printf "\n"


# --- compilation and cleanup --------------------------------------------------
$(NAME):
	@$(CC) $(CFLAGS) $(SRCS) -o $(NAME)
	@echo "$(GREEN)codexion made ✓$(RESET)"

all: $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
	@echo "$(GREEN)m$(RED)e$(GREEN)r$(RED)r$(GREEN)y $(RED)c$(GREEN)h$(RED)r$(GREEN)i$(RED)s$(GREEN)t$(RED)m$(GREEN)a$(RED)s$(RESET)"

%.o: %.c codexion.h
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	clear; rm -f $(NAME)
	@echo "$(GREEN)clean made ✓$(RESET)"

fclean:
	clear; rm -f $(OBJS); rm -f $(NAME)
	@echo "$(GREEN)fclean made ✓✓✓$(RESET)"

re: clean $(NAME)

# --- execution ----------------------------------------------------------------
run: $(NAME)
	@$(NEWLINE); echo "$(WHITE)entering the codexion...$(RESET)"; $(NEWLINE);
	@./$(NAME) $(ST) $(TRUEFALSE); $(NEWLINE)

run--visual: $(NAME)
	clear; echo "$(WHITE)entering the \
	$(GREEN)c$(YELLOW)o$(MAGENTA)l$(CYAN)o$(YELLOW)r$(RED)e$(GREEN)x$(MAGENTA)i$(CYAN)o$(RED)n$(WHITE)...$(RESET)"; \
	$(NEWLINE); sleep 1; ./$(NAME) $(CR) $(VISUAL) $(TRUEFALSE); $(NEWLINE)

ft_run:
	@clear; $(NEWLINE); echo "$(MAGENTA)entering the school...$(RESET)"; sleep 1
	@echo "$(RED)the rush begins!$(RESET)"; $(NEWLINE);
	@./$(NAME) $(FT) $(TRUEFALSE); $(NEWLINE)

frun: re flint
	@$(NEWLINE); echo "$(YELLOW)entering the codexion...$(RESET)"; sleep 1
	@echo "$(CYAN)the codexion begins!$(RESET)"; $(NEWLINE);
	@./$(NAME) $(ST) $(TRUEFALSE); $(NEWLINE)

# --- lint & leaks -------------------------------------------------------------
lint:
	@$(NEWLINE); echo "$(YELLOW)norminetting...$(RESET)"
	@$(NORM) $(TRUEFALSE); $(NEWLINE);

val: $(NAME)
	@$(NEWLINE); echo "$(MAGENTA)valgrinding...$(RESET)"
	@$(VAL) $(VALFLAGS) ./$(NAME) $(LT)
	@echo "$(MAGENTA)valgrinded ✓$(RESET)"; $(NEWLINE); sleep 1

hel: $(NAME)
	@$(NEWLINE); echo "$(RED)helgrinding...$(RESET)"
	@$(VAL) $(HEL) $(HELFLAGS) ./$(NAME) $(LT)
	@echo "$(RED)helgrinded ✓$(RESET)"; $(NEWLINE); sleep 1

flint: lint val hel


.PHONY: this all clean fclean re run run--visual ft_run frun lint val hel flint
