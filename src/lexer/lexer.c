/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkambarb <gkambarb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 10:15:12 by gkambarb          #+#    #+#             */
/*   Updated: 2025/07/24 11:58:52 by gkambarb         ###   ########.fr       */
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

int	is_quotes(char c)
{
	return (c == '\'' || c == '\"');
}

static int	process_token(char *input, int *i, t_token **tokens, int *pre_space)
{
	if (is_whitespace(input[*i]))
	{
		*pre_space = 1;
		(*i)++;
	}
	else if (is_operator_char(input[*i]))
	{
		if (!handle_operator(input, i, tokens, *pre_space))
			return (0);
		*pre_space = 0;
		(*i)++;
	}
	else if (is_quotes(input[*i]))
	{
		if (!handle_quoted_string(input, i, tokens, *pre_space))
			return (0);
		*pre_space = 0;
	}
	else if (!handle_word(input, i, tokens, *pre_space))
		return (0);
	else
		*pre_space = 0;
	return (1);
}

t_token	*lexer(char *input)
{
	int		i;
	int		pre_space;
	t_token	*tokens;

	i = 0;
	tokens = NULL;
	pre_space = 0;
	while (input[i])
	{
		if (!process_token(input, &i, &tokens, &pre_space))
			return (free_tokens(tokens), NULL);
	}
	return (tokens);
}
