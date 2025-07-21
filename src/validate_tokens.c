/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_tokens.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkambarb <gkambarb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 15:35:59 by gkambarb          #+#    #+#             */
/*   Updated: 2025/07/20 00:36:18 by gkambarb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static const char	*token_to_str(int type)
{
	if (type == T_PIPE)
		return ("|");
	if (type == T_REDIR_IN)
		return ("<");
	if (type == T_REDIR_OUT)
		return (">");
	if (type == T_APPEND)
		return (">>");
	if (type == T_HEREDOC)
		return ("<<");
	return ("unknown");
}

static void	print_syntax_error(t_token *token)
{
	const char	*unexpected;
	char		*suffix;
	char		*full_msg;

	if (token)
		unexpected = token_to_str(token->type);
	else
		unexpected = "newline";
	suffix = ft_strjoin(unexpected, "'");
	if (!suffix)
		return ;
	full_msg = ft_strjoin("syntax error near unexpected token `", suffix);
	if (!full_msg)
	{
		free(suffix);
		return ;
	}
	print_error(NULL, full_msg);
	free(suffix);
	free(full_msg);
}

static int	validate_start(t_token *tokens)
{
	t_token	*next;

	if (tokens->type == T_PIPE)
	{
		print_syntax_error(tokens);
		return (0);
	}
	if (tokens->type >= T_REDIR_IN && tokens->type <= T_APPEND)
	{
		next = tokens->next;
		if (!next || next->type != T_WORD)
		{
			print_syntax_error(next);
			return (0);
		}
		if ((tokens->type == T_REDIR_IN || tokens->type == T_HEREDOC)
			&& !tokens->next->next)
		{
			print_syntax_error(NULL);
			return (0);
		}
	}
	return (1);
}

static int	validate_token_sequence(t_token *tokens)
{
	t_token	*current;
	t_token	*next;

	current = tokens;
	while (current)
	{
		next = current->next;
		if (current->type == T_PIPE && (!next || next->type == T_PIPE))
		{
			print_syntax_error(next);
			return (0);
		}
		if ((current->type >= T_REDIR_IN && current->type <= T_APPEND)
			&& (!next || next->type != T_WORD))
		{
			print_syntax_error(next);
			return (0);
		}
		current = current->next;
	}
	return (1);
}

int	validate_tokens(t_token *tokens)
{
	if (!tokens)
		return (1);
	if (!validate_start(tokens))
		return (0);
	if (!validate_token_sequence(tokens))
		return (0);
	return (1);
}
