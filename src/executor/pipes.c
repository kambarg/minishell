/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wuabdull <wuabdull@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 15:49:25 by wuabdull          #+#    #+#             */
/*   Updated: 2025/07/15 15:52:38 by wuabdull         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	setup_pipes(t_command *cmd)
{
	int	pipefd[2];

	if (!cmd->next)
		return ;
	if (pipe(pipefd) == -1)
	{
		print_error("pipe", strerror(errno));
		return ;
	}
	cmd->pipe_fd[1] = pipefd[1];
	cmd->next->pipe_fd[0] = pipefd[0];
}
