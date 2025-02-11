NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror

LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

# READLINE_DIR = $(shell brew --prefix readline)
# READLINE_LIB = -lreadline -lhistory -L $(READLINE_DIR)/lib -lreadline -L$(LIBFT_DIR) -lft
# READLINE_LIB = -lreadline -lhistory -lreadline -Llibft/ -lft
READLINE_LIB = -lreadline

HEADERS = minishell.h 

SOURCES = \
			main.c
			
OBJECTS = $(SOURCES:.c=.o)

%.o: %.c
	$(CC) $(CFLAGS) -Iincludes -c $< -o $@

all: $(LIBFT) $(NAME)

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR) all

$(NAME): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJECTS) $(LIBFT) $(READLINE_LIB)
 
clean:
	$(MAKE) -C $(LIBFT_DIR) clean
	rm -f $(OBJECTS)

fclean: clean
	$(MAKE) -C $(LIBFT_DIR) fclean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re