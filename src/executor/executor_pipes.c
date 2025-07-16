/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_pipes.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wuabdull <wuabdull@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 16:49:34 by wuabdull          #+#    #+#             */
/*   Updated: 2025/07/15 16:49:40 by wuabdull         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	init_pipe_fds_loop(t_command *cmd)
{
	while (cmd)
	{
		cmd->pipe_fd[0] = -1;
		cmd->pipe_fd[1] = -1;
		cmd = cmd->next;
	}
}

void	setup_pipes_loop(t_command *cmd)
{
	while (cmd)
	{
		setup_pipes(cmd);
		cmd = cmd->next;
	}
}

void	close_all_pipes(t_command *cmd)
{
	while (cmd)
	{
		if (cmd->pipe_fd[0] != -1)
			close(cmd->pipe_fd[0]);
		if (cmd->pipe_fd[1] != -1)
			close(cmd->pipe_fd[1]);
		cmd = cmd->next;
	}
}
