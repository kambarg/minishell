#include <../includes/lexer.h>
// #include "../minishell.h" //// Only if needed for global structures/macros
#include <ctype.h> // is alnum, is_space
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

// Expands environment variables in any token
char	*expand_env_variables(const char *input)
{
	size_t	len;
	size_t	i = 0, j;
	size_t	var_start;
	size_t	var_len = 0;
	char	var_name[var_len + 1];
	char	*var_value;

	len = ft_strlen(input);
	char *expanded = malloc(len * 2 + 1); // Extra space for large expansions
	if (!expanded)
	{
		fprintf(stderr, "Memory allocation error\n");
		return (NULL);
	}
	i = 0, j = 0;
	while (input[i])
	{
		if (input[i] == '$' && (ft_isalpha(input[i + 1]) || input[i + 1] == '_'))
		{
			// Start extracting variable name
			var_start = i + 1;
			while (ft_isalnum(input[var_start]) || input[var_start] == '_')
				var_start++;
			// Extract variable name
			var_len = var_start - (i + 1);
			strncpy(var_name, &input[i + 1], var_len);
			var_name[var_len] = '\0';
			// Get environment variable value
			var_value = getenv(var_name);
			if (var_value)
			{
				strcpy(&expanded[j], var_value);
				j += ft_strlen(var_value);
			}
			i = var_start; // Move past variable name
		}
		else
		{
			expanded[j++] = input[i++];
		}
	}
	expanded[j] = '\0';
	return (expanded);
}

char	*extract_quoted_token(const char *input, size_t *index)
{
	size_t	start;
	size_t	length;
	char	*token;
	char	*expanded_token;

	char quote = input[*index]; // Store opening quote (' or ")
	(*index)++;                 // Move past the quote
	start = *index;
	while (input[*index] && input[*index] != quote)
	{
		(*index)++;
	}
	if (input[*index] == '\0')
	{ // Error: Missing closing quote
		fprintf(stderr, "Error: Missing closing quote\n");
		return (NULL);
	}
	length = *index - start;
	(*index)++; // Move past the closing quote
	token = malloc(length + 1);
	if (!token)
	{
		fprintf(stderr, "Memory allocation error\n");
		return (NULL);
	}
	strncpy(token, input + start, length);
	token[length] = '\0';
	// Expand variables **only inside double quotes**
	if (quote == '"')
	{
		expanded_token = expand_env_variables(token);
		free(token);
		return (expanded_token);
	}
	return (token); // Return unchanged if inside single quotes
}

char	**tokenize(char *input)
{
	size_t	input_len;
	char	**tokens;
	size_t	i = 0, token_count;
	size_t	start;
	char	*quoted_token;
	size_t	op_len;
	size_t	token_length;
	char	*raw_token;
	char	*expanded_token;

	if (!input)
		return (NULL);
	input_len = strlen(input);
	tokens = malloc((input_len + 1) * sizeof(char *));
	if (!tokens)
	{
		fprintf(stderr, "Memory allocation error\n");
		return (NULL);
	}
	i = 0, token_count = 0;
	while (i < input_len)
	{
		while (i < input_len && is_space(input[i]))
			i++; // Skip spaces
		if (i >= input_len)
			break ;
		start = i;
		// Handle quoted tokens
		if (input[i] == '"' || input[i] == '\'')
		{
			quoted_token = extract_quoted_token(input, &i);
			if (!quoted_token)
				return (NULL);
			tokens[token_count++] = quoted_token;
		}
		// Handle operators (|, <, >, >>, <<) as separate tokens
		else if (is_operator(input[i]))
		{
			op_len = 1;
			if ((input[i] == '>' || input[i] == '<') && input[i
				+ 1] == input[i])
			{
				op_len = 2; // Handle ">>" or "<<"
			}
			tokens[token_count] = malloc(op_len + 1);
			if (!tokens[token_count])
				return (NULL);
			strncpy(tokens[token_count], &input[i], op_len);
			tokens[token_count][op_len] = '\0';
			token_count++;
			i += op_len;
		}
		// Handle normal words (expand variables)
		else
		{
			while (i < input_len && !is_space(input[i])
				&& !is_operator(input[i]))
			{
				i++;
			}
			token_length = i - start;
			raw_token = malloc(token_length + 1);
			if (!raw_token)
				return (NULL);
			strncpy(raw_token, &input[start], token_length);
			raw_token[token_length] = '\0';
			// Expand variables in unquoted tokens
			expanded_token = expand_env_variables(raw_token);
			free(raw_token);
			tokens[token_count++] = expanded_token;
		}
	}
	tokens[token_count] = NULL;
	return (tokens);
}

// Print tokens
void print_tokens(char **tokens)
{
	int		i;

	if (!tokens)
		return;
	i = 0;
	while (tokens[i])
	{
		printf("Token[%d]: %s\n", i, tokens[i]);
		i++;
	}
}

// Recursively free tokens
void free_tokens(char **tokens)
{
	int		i;

	if (!tokens)
		return;
	i = 0;
	while (tokens[i])
	{
		free(tokens[i]);
		i++;
	}
	free(tokens);
}



// Test function prints tokens
// char input[] = "echo  "Hello    $USER" > file.txt | cat < input.txt";
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
