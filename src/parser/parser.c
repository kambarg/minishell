/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkambarb <gkambarb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 14:35:34 by gkambarb          #+#    #+#             */
/*   Updated: 2025/07/06 14:35:34 by gkambarb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static t_command	*create_command(void)
{
	t_command	*cmd;

	cmd = (t_command *)malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	cmd->args = NULL;
	cmd->arg_count = 0;
	cmd->redirects = NULL;
	cmd->next = NULL;
	cmd->pipe_fd[0] = -1;
	cmd->pipe_fd[1] = -1;
	return (cmd);
}

static void	add_command(t_command **head, t_command *new_cmd)
{
	t_command	*current;

	if (!*head)
	{
		*head = new_cmd;
		return ;
	}
	current = *head;
	while (current->next)
		current = current->next;
	current->next = new_cmd;
}
// static int parse_tokens(t_token *tokens, t_command *current, t_command **commands)
// {
// 	while (tokens)
// 	{
// 		if (tokens->type == T_WORD)
// 		{
// 			if (!add_argument(current, tokens->value, tokens->quote_type))
// 				return (0);
// 		}
// 		else if (tokens->type == T_PIPE)
// 		{
// 			add_command(commands, current);
// 			current = create_command();
// 		}
// 		else if (tokens->type >= T_REDIR_IN && tokens->type <= T_APPEND)
// 		{
// 			if (!handle_redirect(&tokens, current))
// 				return (0);
// 		}
// 		tokens = tokens->next;
// 	}
// 	return(1);
// }
// t_command	*parser(t_token *tokens)
// {
// 	t_command	*commands;
// 	t_command	*current;

// 	commands = NULL;
// 	current = create_command();
// 	if (!parse_tokens(tokens, current, &commands))
// 	{
// 		free(current->args);
// 		free(current->redirects);
// 		free(current);
// 		if (commands)
// 			free_commands(commands); // Assuming free_commands is defined to free the list
// 		return (NULL);
// 	}
// 	add_command(&commands, current);
// 	return (commands);
// }

t_command	*parser(t_token *tokens)
{
	t_command	*commands;
	t_command	*current;

	commands = NULL;
	current = create_command();
	while (tokens)
	{
		if (tokens->type == T_WORD)
		{
			if (!add_argument(current, tokens->value, tokens->quote_type))
				return (NULL);
		}
		else if (tokens->type == T_PIPE)
		{
			add_command(&commands, current);
			current = create_command();
		}
		else if (tokens->type >= T_REDIR_IN && tokens->type <= T_APPEND)
		{
			if (!handle_redirect(&tokens, current))
				return (NULL);
		}
		tokens = tokens->next;
	}
	add_command(&commands, current);
	return (commands);
}
