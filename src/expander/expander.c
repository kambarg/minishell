/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkambarb <gkambarb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 14:09:56 by gkambarb          #+#    #+#             */
/*   Updated: 2025/07/15 14:09:56 by gkambarb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	expand_args(t_command *cmd, t_shell *shell)
{
	int			i;
	char		*expanded;
	t_arg_info	*arg;

	i = 0;
	while (i < cmd->arg_count)
	{
		arg = &cmd->args[i];
		expanded = expand_quoted_string(arg->value, shell, arg->quote_type);
		if (expanded)
		{
			free(arg->value);
			arg->value = expanded;
		}
		else
			print_error("expander", "critical memory allocation failed");
		i++;
	}
}

// Note: redirections are always expanded
static void	expand_redirects(t_command *cmd, t_shell *shell)
{
	t_redirect	*redir;
	char		*expanded;

	redir = cmd->redirects;
	while (redir)
	{
		if (redir->type != REDIR_HEREDOC)
		{
			expanded = expand_quoted_string(redir->file, shell, QUOTE_NONE);
			if (expanded)
			{
				free(redir->file);
				redir->file = expanded;
			}
			else
				print_error("expander", "critical memory allocation failed");
		}
		redir = redir->next;
	}
}

void	expander(t_command *cmd, t_shell *shell)
{
	if (!cmd || !shell)
		return ;
	while (cmd)
	{
		expand_args(cmd, shell);
		expand_redirects(cmd, shell);
		cmd = cmd->next;
	}
}
