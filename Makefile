NAME	= codexion


NB_CODERS	= 5
BURNOUT		= 1200
COMPILE		= 200
DEBUG		= 200
REFACTOR	= 200
NB_COMPILES	= 5
COOLDOWN	= 100
SCHEDULER	= fifo
VISUAL		= --visual

STDRUN	= $(NAME) $(NB_CODERS) $(BURNOUT) $(COMPILE) $(DEBUG) $(REFACTOR) $(NB_COMPILES) $(COOLDOWN) $(SCHEDULER)
CLRRUN	= $(COLOR) $(NB_CODERS) $(BURNOUT) $(COMPILE) $(DEBUG) $(REFACTOR) $(NB_COMPILES) $(COOLDOWN) $(SCHEDULER)
LNTRUN	= $(NAME) 3 2000 200 200 200 3 150 fifo
FT_RUN	= $(NAME) 42 7492 420 372 291 4 226 fifo --visual

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

COLOR	= colorexion
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
	@echo ""
	@echo "      master the Makefile before the Makefile masters you"
	@echo ""
	@echo "      make            this"
	@echo "  ════════════════════════════════════════════════════════════════════"
	@echo "      codexion        compile *.c"
	@echo "      all             compile *.c preserving objects"
	@echo "      clean           remove codexion"
	@echo "      fclean          remove codexion and objects"
	@echo "      re              clean + codexion"
	@echo "  ═════════════════════════════════════════════════════════════════════"
	@echo "      run             execute codexion with standard parameters"
	@echo "      run--visual     execute colorexion with standard parameters"
	@echo "      frun            re + flint + run--visual + clean"
	@echo "  ═════════════════════════════════════════════════════════════════════"
	@echo "      lint            norminette"
	@echo "      val             valgrind --leak-check=full"
	@echo "      hel             valgrind --tool=helgrind --free-is-write=yes"
	@echo "      flint           lint + val + hel"
	@echo ""


# --- compilation and cleanup --------------------------------------------------
$(NAME):
	@$(CC) $(CFLAGS) $(SRCS) -o $(NAME)
	@echo "$(GREEN)codexion made ✓$(RESET)"

$(COLOR):
	@$(CC) $(CFLAGS) $(SRCS) -o $(COLOR)
	@echo "$(GREEN)colorexion made ✓$(RESET)"

all: $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
	@echo "$(GREEN)merry$(RESET) $(RED)christmas$(RESET)"

%.o: %.c codexion.h
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	clear; rm -f $(NAME) $(COLOR)
	@echo "$(GREEN)clean made ✓$(RESET)"

fclean:
	clear; rm -f $(OBJS); rm -f $(NAME) $(COLOR)
	@echo "$(GREEN)fclean made ✓✓✓$(RESET)"

re: clean $(NAME)

# --- execution ----------------------------------------------------------------
run: $(NAME)
	@$(NEWLINE); echo "$(WHITE)entering the codexion...$(RESET)"; sleep 1
	@./$(STDRUN) $(TRUEFALSE); $(NEWLINE)

run--visual: $(COLOR)
	clear; echo "$(BLUE)entering the colorexion...$(RESET)"; sleep 1
	@./$(CLRRUN) $(VISUAL) $(TRUEFALSE); $(NEWLINE)

frun: re flint
	@$(NEWLINE); echo "$(BLUE)entering the codexion...$(RESET)"; sleep 2
	@echo "$(MAGENTA)the codexion begins!$(RESET)"; sleep 1
	@./$(FT_RUN) $(TRUEFALSE); $(NEWLINE)

# --- lint & leaks -------------------------------------------------------------
lint:
	@$(NEWLINE); echo "$(YELLOW)norminetting...$(RESET)"
	@$(NORM) $(TRUEFALSE); $(NEWLINE); sleep 1

val: $(NAME)
	@$(NEWLINE); echo "$(MAGENTA)valgrinding...$(RESET)"
	@$(VAL) $(VALFLAGS) ./$(LNTRUN)
	@echo "$(MAGENTA)valgrinded ✓$(RESET)"; $(NEWLINE); sleep 1

hel: $(NAME)
	@$(NEWLINE); echo "$(RED)helgrinding...$(RESET)"
	@$(VAL) $(HEL) $(HELFLAGS) ./$(LNTRUN)
	@echo "$(RED)helgrinded ✓$(RESET)"; $(NEWLINE); sleep 1

flint: lint val hel


.PHONY: this all clean fclean re run run--visual frun lint val hel flint