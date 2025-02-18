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

// aka ctype.h lib function isspace
int	is_space(char c)
{
	return (c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r'
		|| c == ' ');
}

// Function to split input into tokens
char	**tokenize(char *input)
{
	char	**tokens;
	size_t	i;
	size_t	input_len;
	size_t	token_count;
	size_t	token_length;
	size_t	start;

	if (!input) return NULL;
	input_len = ft_strlen(input);
	tokens = malloc((input_len + 1) * sizeof(char *));
    if (!tokens) 
	{
		ft_putstr_fd("Memory allocation error\n", 2);
        return NULL;
    }
	i = 0;
	token_count = 0;
	while (i < input_len)
	{
		// Skip spaces
		while (i < input_len && is_space(input[i]))
			i++;
		if (i >= input_len) break; // End of input
			
		start = i;
		// Handle operators (|, <, >, >>, <<)
        if (is_operator(input[i])) 
		{
            i++;
            if ((input[start] == '>' || input[start] == '<') && input[i] == input[start]) 
			{
                i++; // Handle ">>" or "<<"
            }
        } 
        // Handle normal words
        else 
		{
            while (i < input_len && !is_space(input[i]) && !is_operator(input[i])) 
			{
                i++;
            }
        }
		// Allocate memory for the token and copy it
        token_length = i - start;
        tokens[token_count] = malloc(token_length + 1);
        if (!tokens[token_count]) 
		{
            ft_putstr_fd("Memory allocation error\n", 2);
            return NULL;
        }
		ft_memcpy(tokens[token_count], &input[start], token_length);
		// ft_strlcpy(tokens[token_count], &input[start], token_length);
        // strncpy(tokens[token_count], &input[start], token_length);
        tokens[token_count][token_length] = '\0'; // Null-terminate the token
        token_count++;
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
