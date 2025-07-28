/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkambarb <gkambarb@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 10:15:12 by gkambarb          #+#    #+#             */
/*   Updated: 2025/07/28 05:25:18 by gkambarb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	process_char_case(char *input, int *i, t_arg_info *cur_arg,
		t_token **tokens)
{
	if (is_whitespace(input[*i]))
		return (whitespace_hun(cur_arg, tokens, i));
	else if (is_operator_char(input[*i]))
		return (operator_hun(cur_arg, tokens, input, i));
	else if (is_quotes(input[*i]))
		return (quotes_hun(cur_arg, input, i));
	else
		return (word_hun(cur_arg, input, i));
}

t_token	*lexer(char *input)
{
	int			i;
	t_token		*tokens;
	t_arg_info	cur_arg;

	i = 0;
	tokens = NULL;
	cur_arg.value = NULL;
	cur_arg.quote_type = QUOTE_NONE;
	while (input[i])
	{
		if (!process_char_case(input, &i, &cur_arg, &tokens))
			return (free_tokens(tokens), NULL);
	}
	if (cur_arg.value)
	{
		add_token(&tokens, create_token(cur_arg.value, T_WORD,
				cur_arg.quote_type));
		free(cur_arg.value);
	}
	return (tokens);
}
