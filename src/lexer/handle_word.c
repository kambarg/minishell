/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_word.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkambarb <gkambarb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 11:17:46 by gkambarb          #+#    #+#             */
/*   Updated: 2025/07/18 10:58:30 by gkambarb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	is_valid_identifier_char(char c)
{
	return (ft_isalnum(c) || c == '_');
}

static int	skip_quoted_value(char *input, int *i)
{
	char	quote;

	if (!is_quotes(input[*i]))
		return (0);
	quote = input[*i];
	(*i)++;
	while (input[*i] && input[*i] != quote)
		(*i)++;
	if (input[*i] == quote)
		(*i)++;
	return (1);
}

static int	parse_assignment_value(char *input, int *i)
{
	if (is_quotes(input[*i]))
	{
		skip_quoted_value(input, i);
	}
	else
	{
		while (input[*i] && !is_whitespace(input[*i])
			&& !is_operator_char(input[*i]))
			(*i)++;
	}
	return (1);
}

static char	*get_word(char *input, int *i)
{
	int		start;
	int		len;
	char	*str;

	start = *i;
	while (input[*i] && !is_whitespace(input[*i])
		&& !is_operator_char(input[*i]) && !is_quotes(input[*i]))
	{
		if (input[*i] == '=' && *i > start
			&& is_valid_identifier_char(input[*i - 1]))
		{
			(*i)++;
			parse_assignment_value(input, i);
			break ;
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

int	handle_word(char *input, int *i, t_token **tokens)
{
	char	*word;

	word = get_word(input, i);
	if (!word)
		return (0);
	add_token(tokens, create_token(word, T_WORD, QUOTE_NONE));
	return (1);
}
