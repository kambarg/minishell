/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkambarb <gkambarb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 20:37:00 by wuabdull          #+#    #+#             */
/*   Updated: 2025/07/28 17:30:34 by gkambarb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	whitespace_hun(t_arg_info *cur_arg, t_token **tokens, int *i)
{
	if (cur_arg->value && cur_arg->value[0])
	{
		add_token(tokens, create_token(cur_arg->value, T_WORD,
				cur_arg->quote_type));
		free(cur_arg->value);
		cur_arg->value = NULL;
		cur_arg->quote_type = QUOTE_NONE;
	}
	(*i)++;
	return (1);
}

int	operator_hun(t_arg_info *cur_arg, t_token **tokens, char *input, int *i)
{
	if (cur_arg->value && cur_arg->value[0])
	{
		add_token(tokens, create_token(cur_arg->value, T_WORD,
				cur_arg->quote_type));
		free(cur_arg->value);
		cur_arg->value = NULL;
		cur_arg->quote_type = QUOTE_NONE;
	}
	if (!handle_operator(input, i, tokens))
		return (0);
	(*i)++;
	return (1);
}

static void	handle_quote_type(t_arg_info *cur_arg, char quote_char)
{
	if (cur_arg->quote_type == QUOTE_NONE)
	{
		if (quote_char == '\'')
			cur_arg->quote_type = QUOTE_SINGLE;
		else if (quote_char == '"')
			cur_arg->quote_type = QUOTE_DOUBLE;
	}
	else if ((cur_arg->quote_type == QUOTE_SINGLE && quote_char != '\'')
		|| (cur_arg->quote_type == QUOTE_DOUBLE && quote_char != '"'))
	{
		cur_arg->quote_type = QUOTE_NONE;
	}
}

int	quotes_hun(t_arg_info *cur_arg, char *input, int *i)
{
	char	*quoted;
	char	*tmp;
	char	quote_char;

	quote_char = input[*i];
	quoted = get_quoted_str(input, i, quote_char);
	if (!quoted)
	{
		free(cur_arg->value);
		cur_arg->value = NULL;
		return (0);
	}
	if (!cur_arg->value)
		cur_arg->value = ft_strdup("");
	tmp = ft_strjoin(cur_arg->value, quoted);
	free(cur_arg->value);
	cur_arg->value = tmp;
	free(quoted);
	handle_quote_type(cur_arg, quote_char);
	return (1);
}

int	word_hun(t_arg_info *cur_arg, char *input, int *i)
{
	int		start;
	int		len;
	char	*word;
	char	*tmp;

	start = *i;
	while (input[*i] && !is_whitespace(input[*i])
		&& !is_operator_char(input[*i]) && !is_quotes(input[*i]))
		(*i)++;
	len = *i - start;
	word = (char *)malloc(len + 1);
	if (!word)
		return (0);
	ft_strlcpy(word, input + start, len + 1);
	if (!cur_arg->value)
		cur_arg->value = ft_strdup("");
	tmp = ft_strjoin(cur_arg->value, word);
	free(cur_arg->value);
	cur_arg->value = tmp;
	free(word);
	if (cur_arg->quote_type != QUOTE_NONE)
		cur_arg->quote_type = QUOTE_NONE;
	return (1);
}
