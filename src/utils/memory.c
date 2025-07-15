/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkambarb <gkambarb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 23:07:29 by gkambarb          #+#    #+#             */
/*   Updated: 2025/07/08 23:07:31 by gkambarb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	free_array(char **array)
{
	int	i;

	if (!array)
		return ;
	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

void	free_tokens(t_token *tokens)
{
	t_token	*temp;

	while (tokens)
	{
		temp = tokens;
		tokens = tokens->next;
		if (temp->value)
			free(temp->value);
		free(temp);
	}
}

// Close heredoc temp file descriptor if open
static void	free_redirects(t_redirect *redirects)
{
	t_redirect	*temp;

	while (redirects)
	{
		temp = redirects;
		redirects = redirects->next;
		if (temp->file)
			free(temp->file);
		if (temp->fd != -1)
			close(temp->fd);
		free(temp);
	}
}

static void	free_args(t_arg_info *args, int arg_count)
{
	int	i;

	i = 0;
	while (i < arg_count)
	{
		if (args[i].value)
			free(args[i].value);
		i++;
	}
	free(args);
}

// Close any open pipe file descriptors
void	free_commands(t_command *commands)
{
	t_command	*temp;

	while (commands)
	{
		temp = commands;
		commands = commands->next;
		if (temp->args)
			free_args(temp->args, temp->arg_count);
		if (temp->redirects)
			free_redirects(temp->redirects);
		if (temp->pipe_fd[0] != -1)
			close(temp->pipe_fd[0]);
		if (temp->pipe_fd[1] != -1)
			close(temp->pipe_fd[1]);
		free(temp);
	}
}
