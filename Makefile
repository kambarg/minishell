NAME = minishell

CC = gcc
CFLAGS = -Wall -Wextra -Werror
LDFLAGS = -L/opt/vagrant/embedded/lib -lreadline -lhistory
INCLUDES = -I./includes -I$(LIBFT_DIR) -I/opt/vagrant/embedded/include/readline/readline.h 
# -I/usr/local/include 
RM = rm -f

SRCS = 	src/main.c \
		src/init_shell.c \
		src/run_shell.c \
		src/builtins/echo.c \
		src/builtins/cd.c \
		src/builtins/pwd.c \
		src/builtins/export.c \
		src/builtins/export_helper.c \
		src/builtins/export_utils.c \
		src/builtins/unset.c \
		src/builtins/env.c \
		src/builtins/exit.c \
		src/lexer/lexer.c \
		src/lexer/token.c \
		src/lexer/handle_operator.c \
		src/lexer/handle_quoted_string.c \
		src/lexer/handle_word.c \
		src/parser/parser.c \
		src/parser/args.c \
		src/parser/handle_redirect.c \
		src/expander/expander.c \
		src/expander/expand_var.c \
		src/expander/expand_string.c \
		src/executor/executor.c \
		src/executor/redirections.c \
		src/executor/pipes.c \
		src/utils/signals.c \
		src/utils/get_env_value.c \
		src/utils/set_env_value.c \
		src/utils/error.c \
		src/utils/memory.c \
		src/utils/temp_files.c\
		src/utils/temp_path.c\
		src/executor/executor_args.c \
		src/executor/executor_builtin.c \
		src/executor/executor_path.c \
		src/executor/executor_pipes.c \
		src/executor/executor_external.c \
		src/executor/executor_loop.c
		

OBJS = $(SRCS:.c=.o)

LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

all: $(LIBFT) $(NAME)

$(LIBFT):
	@make -C $(LIBFT_DIR)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -L$(LIBFT_DIR) -lft $(LDFLAGS) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	@make -C $(LIBFT_DIR) clean
	$(RM) $(OBJS)

fclean: clean
	@make -C $(LIBFT_DIR) fclean
	$(RM) $(NAME)

re: fclean all

leaks : debug
		valgrind --show-leak-kinds=all --leak-check=full --suppressions=vg_rlsuppressions.supp ./$(NAME)

debug : C_FLAGS += -g3
debug : re

.PHONY: all clean fclean re leaks debug