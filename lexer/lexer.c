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

// Function to extract a token inside quotes
char	*extract_quoted_token(const char *input, size_t *index)
{
	size_t	start;
	char	*token;

	char quote = input[*index]; // Store the opening quote (' or ")
	(*index)++;                 // Move past the opening quote
	start = *index;
	while (input[*index] && input[*index] != quote)
	{
		(*index)++;
	}
	// If we reached the end without finding the closing quote, it's an error
	if (input[*index] == '\0')
	{
		fprintf(stderr, "Error: Missing closing quote\n");
		return (NULL);
	}
	size_t length = *index - start; // Get length of quoted content
	(*index)++;                     // Move past the closing quote
	// Allocate memory for the token and copy it
	token = malloc(length + 1);
	if (!token)
	{
		fprintf(stderr, "Memory allocation error\n");
		return (NULL);
	}
	ft_strlcpy(token, input + start, length + 1);
	// strncpy(token, input + start, length);
	token[length + 1] = '\0'; // Null-terminate the token
	return (token);
}

#include <ctype.h>
char **tokenize(char *input) {
    if (!input) return NULL;

    size_t input_len = strlen(input);
    char **tokens = malloc((input_len + 1) * sizeof(char *));
    if (!tokens) {
        fprintf(stderr, "Memory allocation error\n");
        return NULL;
    }

    size_t i = 0, token_count = 0;
    while (i < input_len) {
        while (i < input_len && isspace(input[i])) i++; // Skip spaces

        if (i >= input_len) break;

        size_t start = i;

        // Handle quoted tokens
        if (input[i] == '"' || input[i] == '\'') {
            char *quoted_token = extract_quoted_token(input, &i);
            if (!quoted_token) return NULL;
            tokens[token_count++] = quoted_token;
        }
        // Handle operators (|, <, >, >>, <<) as separate tokens
        else if (is_operator(input[i])) {
            size_t op_len = 1;
            if ((input[i] == '>' || input[i] == '<') && input[i + 1] == input[i]) {
                op_len = 2; // Handle ">>" or "<<"
            }

            tokens[token_count] = malloc(op_len + 1);
            if (!tokens[token_count]) return NULL;
            strncpy(tokens[token_count], &input[i], op_len);
            tokens[token_count][op_len] = '\0';
            token_count++;
            i += op_len;
        }
        // Handle normal words
        else {
            while (i < input_len && !isspace(input[i]) && !is_operator(input[i])) {
                i++;
            }
            size_t token_length = i - start;
            tokens[token_count] = malloc(token_length + 1);
            if (!tokens[token_count]) return NULL;
            strncpy(tokens[token_count], &input[start], token_length);
            tokens[token_count][token_length] = '\0';
            token_count++;
        }
    }

    tokens[token_count] = NULL;
    return tokens;
}

// Function to split input into tokens
// char	**tokenize(char *input)
// {
// 	char	**tokens;
// 	size_t	i;
// 	size_t	input_len;
// 	size_t	token_count;
// 	size_t	token_length;
// 	size_t	start;
// 	char	*quoted_token;

// 	if (!input)
// 		return (NULL);
// 	input_len = ft_strlen(input);
// 	tokens = malloc((input_len + 1) * sizeof(char *));
// 	if (!tokens)
// 	{
// 		ft_putstr_fd("Memory allocation error\n", 2);
// 		return (NULL);
// 	}
// 	i = 0;
// 	token_count = 0;
// 	while (i < input_len)
// 	{
// 		// Skip spaces
// 		while (i < input_len && is_space(input[i]))
// 			i++;
// 		if (i >= input_len)
// 			break ; // End of input
// 		start = i;
// 		// Handle quoted tokens
// 		if (input[i] == '"' || input[i] == '\'')
// 		{
// 			quoted_token = extract_quoted_token(input, &i);
// 			if (!quoted_token)
// 				return (NULL);
// 			tokens[token_count++] = quoted_token;
// 		}
// 		// Handle operators (|, <, >, >>, <<)
// 		if (is_operator(input[i]))
// 		{
// 			i++;
// 			if ((input[start] == '>' || input[start] == '<')
// 				&& input[i] == input[start])
// 			{
// 				i++; // Handle ">>" or "<<"
// 			}
// 		}
// 		// Handle normal words
// 		else
// 		{
// 			while (i < input_len && !is_space(input[i])
// 				&& !is_operator(input[i]))
// 			{
// 				i++;
// 			}
// 			// Allocate memory for the token and copy it
// 			token_length = i - start;
// 			tokens[token_count] = malloc(token_length + 1);
// 			if (!tokens[token_count])
// 			{
// 				ft_putstr_fd("Memory allocation error\n", 2);
// 				return (NULL);
// 			}
// 			ft_strlcpy(tokens[token_count], &input[start], token_length + 1);
// 			tokens[token_count][token_length + 1] = '\0';
// 			token_count++;
// 		}
// 	}
// 	tokens[token_count] = NULL; // Null-terminate the token array
// 	return (tokens);
// }

// Test function prints tokens
// char input[] = "echo  Hello > file.txt | cat < input.txt";
int	lexer(char *input)
{
	char	**tokens;
	int		i;

	tokens = tokenize(input);
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
