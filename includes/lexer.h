#ifndef LEXER_H
# define LEXER_H

# include "../libft/libft.h"

int		lexer(char *input); // Test function prints tokens
char	**tokenize(char *input); // Lexer function prototype
void	print_tokens(char **tokens);
void	free_tokens(char **tokens);

#endif