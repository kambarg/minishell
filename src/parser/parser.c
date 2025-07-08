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

static t_arg_info	*create_argument(char *value, int quote_type)
{
	t_arg_info	*arg;

	arg = (t_arg_info *)malloc(sizeof(t_arg_info));
	if (!arg)
		return (NULL);
	arg->value = ft_strdup(value);
	if (!arg->value)
	{
		free(arg);
		return (NULL);
	}
	arg->quote_type = quote_type;
	return (arg);
}

static int	add_argument(t_command *cmd, char *value, int quote_type)
{
	t_arg_info	*new_args;
	t_arg_info	*new_arg;
	int			i;

	new_arg = create_argument(value, quote_type);
	if (!new_arg)
		return (0);
	new_args = (t_arg_info *)malloc(sizeof(t_arg_info) * (cmd->arg_count + 1));
	if (!new_args)
	{
		free(new_arg->value);
		free(new_arg);
		return (0);
	}
	i = 0;
	while (i < cmd->arg_count)
	{
		new_args[i] = cmd->args[i];
		i++;
	}
	new_args[i] = *new_arg;
	free(new_arg);
	free(cmd->args);
	cmd->args = new_args;
	cmd->arg_count++;
	return (1);
}

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
