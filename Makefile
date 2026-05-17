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

ST	= $(NB_CODERS) $(BURNOUT) $(COMPILE) $(DEBUG) $(REFACTOR) $(NB_COMPILES) $(COOLDOWN) $(SCHEDULER)
CR	= $(NB_CODERS) $(BURNOUT) $(COMPILE) $(DEBUG) $(REFACTOR) $(NB_COMPILES) $(COOLDOWN) $(SCHEDULER)
LT	= 3 2000 200 200 200 3 150 fifo
FT	= 42 7492 420 372 291 4 226 fifo --visual

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
	@echo ""
	@echo "    master the Makefile before the Makefile masters you"
	@echo ""
	@echo "    ╔                                                 ╗" 
	@echo "    ║  st :  $(ST)            ║"
	@echo "    ║  lt :  $(LT)            ║"
	@echo "    ║  ft : $(FT)   ║"
	@echo "    ╚                                                 ╝"
	@echo ""
	@echo "      make            this"
	@echo "  ═════════════════════════════════════════════════════════════════════"
	@echo "      codexion        compile *.c -Wall -Werror -Wextra -pthread"
	@echo "      all             compile *.c preserving objects"
	@echo "      clean           remove binary"
	@echo "      fclean          remove binary and objects"
	@echo "      re              clean + codexion"
	@echo "  ═════════════════════════════════════════════════════════════════════"
	@echo "      run             execute codexion    [./$(NAME) st]"
	@echo "      run--visual     execute colorexion  [./$(NAME) st --visual]"
	@echo "      frun            re + flint + run--visual + clean"
	@echo "  ═════════════════════════════════════════════════════════════════════"
	@echo "      lint            norminette"
	@echo "      val             valgrind --leak-check=full  [./$(NAME) lt]"
	@echo "      hel             valgrind --tool=helgrind    [./$(NAME) lt]"
	@echo "      flint           lint + val + hel"
	@echo ""


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
	@$(NEWLINE); echo "$(WHITE)entering the codexion...$(RESET)"; sleep 1
	@./$(NAME) $(ST) $(TRUEFALSE); $(NEWLINE)

run--visual: $(NAME)
	clear; echo "$(BLUE)entering the colorexion...$(RESET)"; sleep 1
	@./$(NAME) $(CR) $(VISUAL) $(TRUEFALSE); $(NEWLINE)

frun: re flint
	@$(NEWLINE); echo "$(BLUE)entering the codexion...$(RESET)"; sleep 2
	@echo "$(MAGENTA)the codexion begins!$(RESET)"; sleep 1
	@./$(NAME) $(FT) $(TRUEFALSE); $(NEWLINE)

# --- lint & leaks -------------------------------------------------------------
lint:
	@$(NEWLINE); echo "$(YELLOW)norminetting...$(RESET)"
	@$(NORM) $(TRUEFALSE); $(NEWLINE); sleep 1

val: $(NAME)
	@$(NEWLINE); echo "$(MAGENTA)valgrinding...$(RESET)"
	@$(VAL) $(VALFLAGS) ./$(NAME) $(LT)
	@echo "$(MAGENTA)valgrinded ✓$(RESET)"; $(NEWLINE); sleep 1

hel: $(NAME)
	@$(NEWLINE); echo "$(RED)helgrinding...$(RESET)"
	@$(VAL) $(HEL) $(HELFLAGS) ./$(NAME) $(LT)
	@echo "$(RED)helgrinded ✓$(RESET)"; $(NEWLINE); sleep 1

flint: lint val hel


.PHONY: this all clean fclean re run run--visual frun lint val hel flint