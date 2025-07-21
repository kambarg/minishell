/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wuabdull <wuabdull@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 10:15:12 by gkambarb          #+#    #+#             */
/*   Updated: 2025/07/21 20:36:54 by wuabdull         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	process_char_case(char *input, int *i,
	char **arg_buf, t_token **tokens)
{
	if (is_whitespace(input[*i]))
		return (whitespace_hun(arg_buf, tokens, i));
	else if (is_operator_char(input[*i]))
		return (operator_hun(arg_buf, tokens, input, i));
	else if (is_quotes(input[*i]))
		return (quotes_hun(arg_buf, input, i));
	else
		return (word_hun(arg_buf, input, i));
}

t_token	*lexer(char *input)
{
	int		i;
	t_token	*tokens;
	char	*arg_buf;

	i = 0;
	tokens = NULL;
	arg_buf = NULL;
	while (input[i])
	{
		if (!process_char_case(input, &i, &arg_buf, &tokens))
			return (free_tokens(tokens), NULL);
	}
	if (arg_buf)
		add_token(&tokens, create_token(arg_buf, T_WORD, QUOTE_NONE));
	free(arg_buf);
	return (tokens);
}
