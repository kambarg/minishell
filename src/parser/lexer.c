#include "../../includes/minishell.h"

int	is_whitespace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

int	is_operator_char(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

char	*get_quoted_str(char *input, int *i, char quote)
{
	int		start;
	int		len;
	char	*str;

	(*i)++;
	start = *i;
	while (input[*i] && input[*i] != quote)
		(*i)++;
	if (!input[*i])
		return (NULL);
	len = *i - start;
	str = (char *)malloc(sizeof(char) * (len + 1));
	if (!str)
		return (NULL);
	ft_strlcpy(str, input + start, len + 1);
	(*i)++;
	return (str);
}

char	*get_word(char *input, int *i)
{
	int		start;
	int		len;
	char	*str;

	start = *i;
	while (input[*i] && !is_whitespace(input[*i]) && !is_operator_char(input[*i]))
	{
		if (input[*i] == '\'' || input[*i] == '\"')
		{
			str = get_quoted_str(input, i, input[*i]);
			if (!str)
				return (NULL);
			return (str);
		}
		(*i)++;
	}
	len = *i - start;
	str = (char *)malloc(sizeof(char) * (len + 1));
	if (!str)
		return (NULL);
	ft_strlcpy(str, input + start, len + 1);
	return (str);
}

t_token	*create_token(char *value, int type)
{
	t_token	*token;

	token = (t_token *)malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->value = value;
	token->type = type;
	token->next = NULL;
	return (token);
}

void	add_token(t_token **head, t_token *new_token)
{
	t_token	*current;

	if (!*head)
	{
		*head = new_token;
		return ;
	}
	current = *head;
	while (current->next)
		current = current->next;
	current->next = new_token;
}

static int	handle_operator(char *input, int *i, t_token **tokens)
{
	char	*value;
	int		type;

	type = 0;
	if (input[*i] == '|')
		type = T_PIPE;
	else if (input[*i] == '<')
	{
		if (input[*i + 1] == '<')
		{
			(*i)++;
			type = T_HEREDOC;
		}
		else
			type = T_REDIR_IN;
	}
	else if (input[*i] == '>')
	{
		if (input[*i + 1] == '>')
		{
			(*i)++;
			type = T_APPEND;
		}
		else
			type = T_REDIR_OUT;
	}
	value = ft_substr(input, *i - (type == T_HEREDOC || type == T_APPEND), \
			(type == T_HEREDOC || type == T_APPEND) ? 2 : 1);
	add_token(tokens, create_token(value, type));
	return (1);
}

t_token	*lexer(char *input)
{
	int		i;
	char	*word;
	t_token	*tokens;

	i = 0;
	tokens = NULL;
	while (input[i])
	{
		if (is_whitespace(input[i]))
			i++;
		else if (is_operator_char(input[i]))
		{
			if (!handle_operator(input, &i, &tokens))
				return (NULL);
			i++;
		}
		else
		{
			word = get_word(input, &i);
			if (!word)
				return (NULL);
			add_token(&tokens, create_token(word, T_WORD));
		}
	}
	return (tokens);
} 