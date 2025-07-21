NAME = minishell

CC = gcc

# Compiler flags: enable warnings, define USE_LIBEDIT for libedit compatibility, and include headers
CFLAGS = -Wall -Wextra -Werror \
	-DUSE_LIBEDIT \
	-I./includes \
	-I$(LIBFT_DIR)

# Linker flags: link against libft and macOS libedit
LDFLAGS = -L$(LIBFT_DIR) -lft \
	-ledit

# Libft directory and archive
LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

# Source files
SRCS =  \
	src/main.c \
	src/init_shell.c \
	src/run_shell.c \
	src/validate_tokens.c \
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
	src/lexer/is_functions.c \
	src/lexer/lexer_utils.c \
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
	src/utils/temp_files.c \
	src/utils/temp_path.c \
	src/executor/executor_args.c \
	src/executor/executor_builtin.c \
	src/executor/executor_path.c \
	src/executor/executor_pipes.c \
	src/executor/executor_external.c \
	src/executor/executor_loop.c

# Object files
OBJS = $(SRCS:.c=.o)

# Default target: build libft then minishell
all: $(LIBFT) $(NAME)

# Build libft
$(LIBFT):
	@make -C $(LIBFT_DIR)

# Link objects into the final binary
$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o $(NAME)

# Compile each source file
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Remove object files
clean:
	@make -C $(LIBFT_DIR) clean
	$(RM) $(OBJS)

# Remove all generated files
fclean: clean
	@make -C $(LIBFT_DIR) fclean
	$(RM) $(NAME)

# Rebuild from scratch
re: fclean all

# Leak checking target
leaks: debug
	valgrind --show-leak-kinds=all \
		--leak-check=full \
		--suppressions=vg_rlsuppressions.supp ./$(NAME)

# Debug build with symbols
debug: CFLAGS += -g3
debug: re

.PHONY: all clean fclean re leaks debug
