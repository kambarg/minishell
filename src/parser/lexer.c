/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkambarb <gkambarb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 10:15:12 by gkambarb          #+#    #+#             */
/*   Updated: 2025/07/05 03:40:44 by gkambarb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	{
		print_error(NULL, "syntax error: unclosed quotes");
		return (NULL);
	}
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
	while (input[*i] && !is_whitespace(input[*i]) && !is_operator_char(input[*i]) && input[*i] != '\'' && input[*i] != '\"')
		(*i)++;
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
	token->quote_type = QUOTE_NONE;
	token->next = NULL;
	return (token);
}

t_token	*create_token_with_quote(char *value, int type, int quote_type)
{
	t_token	*token;

	token = (t_token *)malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->value = value;
	token->type = type;
	token->quote_type = quote_type;
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

// Print "syntax error near unexpected token" if
// pipe followed by nothing or another pipe
// redirection without target
int	validate_tokens(t_token *tokens)
{
	t_token	*current;
	t_token	*next;

	if (!tokens)
		return (1);
	current = tokens;
	while (current)
	{
		next = current->next;
		if (current->type == T_PIPE && (!next || next->type == T_PIPE))
		{
			print_error(NULL, "syntax error near unexpected token `|'");
			return (0);
		}
		if ((current->type >= T_REDIR_IN && current->type <= T_APPEND) && 
		    (!next || next->type != T_WORD))
		{
			print_error(NULL, "syntax error near unexpected token");
			return (0);
		}
		current = current->next;
	}
	return (1);
}

static int	handle_regular_word(char *input, int *i, t_token **tokens)
{
	char	*word;

	word = get_word(input, i);
	if (!word)
		return (0);
	add_token(tokens, create_token(word, T_WORD));
	return (1);
}

static int	handle_quoted_string(char *input, int *i, t_token **tokens)
{
	char	*word;
	int		quote_type;

	if (input[*i] == '\'')
		quote_type = QUOTE_SINGLE;
	else
		quote_type = QUOTE_DOUBLE;
	word = get_quoted_str(input, i, input[*i]);
	if (!word)
		return (0);
	add_token(tokens, create_token_with_quote(word, T_WORD, quote_type));
	return (1);
}

t_token	*lexer(char *input)
{
	int		i;
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
		else if (input[i] == '\'' || input[i] == '\"')
		{
			if (!handle_quoted_string(input, &i, &tokens))
				return (NULL);
		}
		else if (!handle_regular_word(input, &i, &tokens))
			return (NULL);
	}
	return (tokens);
}


// t_token	*lexer(char *input)
// {
// 	int		i;
// 	char	*word;
// 	t_token	*tokens;
// 	int		quote_type;

// 	i = 0;
// 	tokens = NULL;
// 	while (input[i])
// 	{
// 		if (is_whitespace(input[i]))
// 			i++;
// 		else if (is_operator_char(input[i]))
// 		{
// 			if (!handle_operator(input, &i, &tokens))
// 				return (NULL);
// 			i++;
// 		}
// 		else if (input[i] == '\'' || input[i] == '\"')
// 		{
// 			if (input[i] == '\'')
// 				quote_type = QUOTE_SINGLE;
// 			else
// 				quote_type = QUOTE_DOUBLE;
// 			word = get_quoted_str(input, &i, input[i]);
// 			if (!word)
// 				return (NULL);
// 			add_token(&tokens, create_token_with_quote(word, T_WORD, quote_type));
// 		}
// 		else
// 		{
// 			word = get_word(input, &i);
// 			if (!word)
// 				return (NULL);
// 			add_token(&tokens, create_token(word, T_WORD));
// 		}
// 	}
// 	return (tokens);
// } 
