/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirect.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkambarb <gkambarb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 14:37:57 by gkambarb          #+#    #+#             */
/*   Updated: 2025/07/19 17:24:39 by gkambarb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// Initialize fd to -1 (not used by default) 
static t_redirect	*create_redirect(int type, char *file, int quote_type)
{
	t_redirect	*redir;

	redir = (t_redirect *)malloc(sizeof(t_redirect));
	if (!redir)
		return (NULL);
	redir->type = type;
	redir->file = ft_strdup(file);
	redir->fd = -1;
	redir->quote_type = quote_type;
	redir->next = NULL;
	return (redir);
}

static void	add_redirect(t_command *cmd, t_redirect *new_redir)
{
	t_redirect	*current;

	if (!cmd->redirects)
	{
		cmd->redirects = new_redir;
		return ;
	}
	current = cmd->redirects;
	while (current->next)
		current = current->next;
	current->next = new_redir;
}

int	handle_redirect(t_token **token, t_command *cmd)
{
	int		type;
	char	*file;
	int		quote_type;

	if ((*token)->type == T_REDIR_IN)
		type = REDIR_IN;
	else if ((*token)->type == T_REDIR_OUT)
		type = REDIR_OUT;
	else if ((*token)->type == T_HEREDOC)
		type = REDIR_HEREDOC;
	else
		type = REDIR_APPEND;
	*token = (*token)->next;
	if (!*token || (*token)->type != T_WORD)
	{
		print_error(NULL, "syntax error near unexpected token");
		return (0);
	}
	file = (*token)->value;
	quote_type = (*token)->quote_type;
	add_redirect(cmd, create_redirect(type, file, quote_type));
	return (1);
}
