/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_operator.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: worodhazem <worodhazem@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 02:29:08 by gkambarb          #+#    #+#             */
/*   Updated: 2025/07/20 22:31:49 by worodhazem       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	handle_input_redirect(char *input, int *i)
{
	if (input[*i + 1] == '<')
	{
		(*i)++;
		return (T_HEREDOC);
	}
	return (T_REDIR_IN);
}

static int	handle_output_redirect(char *input, int *i)
{
	if (input[*i + 1] == '>')
	{
		(*i)++;
		return (T_APPEND);
	}
	return (T_REDIR_OUT);
}

static int	get_operator_type(char *input, int *i)
{
	if (input[*i] == '|')
		return (T_PIPE);
	else if (input[*i] == '<')
		return (handle_input_redirect(input, i));
	else if (input[*i] == '>')
		return (handle_output_redirect(input, i));
	return (0);
}

int	handle_operator(char *input, int *i, t_token **tokens)
{
	char	*value;
	int		type;

	type = get_operator_type(input, i);
	if (type == T_HEREDOC || type == T_APPEND)
		value = ft_substr(input, *i - 1, 2);
	else
		value = ft_substr(input, *i, 1);
	add_token(tokens, create_token(value, type, QUOTE_NONE));
	free(value);
	return (1);
}
