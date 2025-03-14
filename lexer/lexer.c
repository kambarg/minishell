#include <../includes/lexer.h>
// #include "../minishell.h" //// Only if needed for global structures/macros
#include <ctype.h> // is alnum, is_space
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Check if a character is a special operator
int	is_operator(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

// Count special operators and calculate number of tokens
int token_count(char *input)
{
	int count;
	int i;

	i = 0;
	count = 0;
	while (input[i] != '\0')
	{
		if (is_operator(input[i]) == 1 && is_operator(input[i+1]) == 1)
		{
			count++;
			i++;
		}
		else if (is_operator(input[i]) == 1 && is_operator(input[i+1]) == 0)
			count++;
		i++;
	}
	return (count*2 + 1);
}

// Recursively free tokens
char **free_tokens(char **tokens, int j)
{
	if (!tokens)
		return (NULL);
	while (j > 0)
	{
		free(tokens[j - 1]);
		j--;
	}
	free(tokens);
	return (NULL);
}

char *token_oper_assign(char *input, int *i, int *count, char **tokens)
{
	if (input[*i] == '<' && input[*i+1] == '<')
	{
		tokens[*count] = ft_strdup("<<");
		*i++;
	}
	else if (input[*i] == '>' && input[*i+1] == '>')
	{
		tokens[*count] = ft_strdup(">>");
		*i++;
	}
	else if (input[*i] == '<' && input[*i+1] != '<')
		tokens[*count] = ft_strdup("<");
	else if (input[*i] == '>' && input[*i+1] != '>')
		tokens[*count] = ft_strdup(">");
	else if (input[*i] == '|')
		tokens[*count] = ft_strdup("|");
	else
	{
		ft_putstr_fd("Logical error with oper assignment!\n", 2);
		return (NULL);
	}
	*i++;
	return (tokens[*count++]);
}

char *token_cmd_assign(char *input, int *i, int *count, char **tokens)
{
	int		t_cmd_start;
	int		t_cmd_end;

	t_cmd_start = *i;
	while (is_operator(input[*i]) != 1 && input[*i] != '\0')
		*i++;
	t_cmd_end = *i - 1;
	token[*count] = (char *)malloc((t_cmd_end - t_cmd_start + 2) * sizeof(char));
	if (!token[*count])
		free_tokens(tokens, *i);
	token[*count][t_cmd_end - t_cmd_start + 1] = '\0';
	while(t_cmd_end >= t_cmd_start)
	{
		tokens[*count][t_cmd_end - t_cmd_start] = input[t_cmd_end];
		t_cmd_end--;
	}
	return(tokens[*count++]);
}



// Split input string by special operators
char	**tokenize(char *input)
{
	char	**tokens;
	int		i;
	int		count;

	if (!input)
		return (NULL);
	count = token_count(input);
	tokens = malloc(count * sizeof(char *));
	if (!tokens)
	{
		ft_putstr_fd("Memory allocation error\n", 2);
		return (NULL);
	}
	i = 0;
    while (input[i] != '\0') 
	{   
		if (is_operator(input[i]))
			token_oper_assign(input, &i, &count, tokens);
		else
			token_cmd_assign(input, &i, &count, tokens);
    }
    return (tokens);
}
