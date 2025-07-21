/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wuabdull <wuabdull@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 20:37:00 by wuabdull          #+#    #+#             */
/*   Updated: 2025/07/21 20:39:10 by wuabdull         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	whitespace_hun(char **arg_buf, t_token **tokens, int *i)
{
	if (*arg_buf && (*arg_buf)[0])
	{
		add_token(tokens, create_token(*arg_buf, T_WORD, QUOTE_NONE));
		free(*arg_buf);
		*arg_buf = NULL;
	}
	(*i)++;
	return (1);
}

int	operator_hun(char **arg_buf, t_token **tokens, char *input, int *i)
{
	if (*arg_buf && (*arg_buf)[0])
	{
		add_token(tokens, create_token(*arg_buf, T_WORD, QUOTE_NONE));
		free(*arg_buf);
		*arg_buf = NULL;
	}
	if (!handle_operator(input, i, tokens))
		return (0);
	(*i)++;
	return (1);
}

int	quotes_hun(char **arg_buf, char *input, int *i)
{
	char	*quoted;
	char	*tmp;

	quoted = get_quoted_str(input, i, input[*i]);
	if (!quoted)
	{
		free(*arg_buf);
		*arg_buf = NULL;
		return (0);
	}
	if (!*arg_buf)
		*arg_buf = ft_strdup("");
	tmp = ft_strjoin(*arg_buf, quoted);
	free(*arg_buf);
	*arg_buf = tmp;
	free(quoted);
	return (1);
}

int	word_hun(char **arg_buf, char *input, int *i)
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
	if (!*arg_buf)
		*arg_buf = ft_strdup("");
	tmp = ft_strjoin(*arg_buf, word);
	free(*arg_buf);
	*arg_buf = tmp;
	free(word);
	return (1);
}
