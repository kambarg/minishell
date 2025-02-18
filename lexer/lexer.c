#include <../includes/lexer.h>
// #include "../minishell.h" //// Only if needed for global structures/macros
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TOKENS 100

// Function to check if a character is a special operator
int	is_operator(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

// Function to check if a character is a white space (like ctype.h lib function isspace)
int	is_space(char c)
{
	return (c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r'
		|| c == ' ');
}

// Function to split input into tokens
char	**tokenize(char *input)
{
	char	**tokens;
	int		i;
	int		j;
	int		token_count;

	tokens = malloc(MAX_TOKENS * sizeof(char *));
	i = 0;
	j = 0;
	token_count = 0;
	char token[256]; // Temporary storage for each token
	while (input[i])
	{
		// Skip spaces
		if (is_space(input[i]))
		{
			i++;
			continue ;
		}
		j = 0;
		// Handle operators (|, <, >)
		if (is_operator(input[i]))
		{
			token[j++] = input[i++];
			if ((token[0] == '>' || token[0] == '<') && input[i] == token[0])
			{
				token[j++] = input[i++]; // Handle ">>" or "<<"
			}
		}
		// Handle normal words
		else
		{
			while (input[i] && !is_space(input[i]) && !is_operator(input[i]))
			{
				token[j++] = input[i++];
			}
		}
		token[j] = '\0'; // Null-terminate the token
		tokens[token_count++] = ft_strdup(token);
	}
	tokens[token_count] = NULL; // Null-terminate the token array
	return (tokens);
}

// Test function prints tokens
// char input[] = "echo  Hello > file.txt | cat < input.txt";
int	lexer(char *input)
{
	char	**tokens = tokenize(input);
	int	i;

	i = 0;
	while (tokens[i])
	{
		printf("Token[%d]: %s\n", i, tokens[i]);
		free(tokens[i]);
		i++;
	}
	free(tokens);
	return (0);
}
