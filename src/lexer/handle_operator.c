/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_operator.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkambarb <gkambarb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 02:29:08 by gkambarb          #+#    #+#             */
/*   Updated: 2025/07/24 11:06:39 by gkambarb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	handle_operator(char *input, int *i, t_token **tokens, int pre_space)
{
	if (input[*i] == '|')
		add_token(tokens, create_token(ft_strdup("|"), T_PIPE, QUOTE_NONE, pre_space));
	else if (input[*i] == '<' && input[*i + 1] == '<')
	{
		add_token(tokens, create_token(ft_strdup("<<"), T_HEREDOC, QUOTE_NONE, pre_space));
		(*i)++;
	}
	else if (input[*i] == '>' && input[*i + 1] == '>')
	{
		add_token(tokens, create_token(ft_strdup(">>"), T_APPEND, QUOTE_NONE, pre_space));
		(*i)++;
	}
	else if (input[*i] == '<')
		add_token(tokens, create_token(ft_strdup("<"), T_REDIR_IN, QUOTE_NONE, pre_space));
	else if (input[*i] == '>')
		add_token(tokens, create_token(ft_strdup(">"), T_REDIR_OUT, QUOTE_NONE, pre_space));
	else
		return (0);
	return (1);
}
