/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_builtin.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wuabdull <wuabdull@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 16:48:42 by wuabdull          #+#    #+#             */
/*   Updated: 2025/07/21 11:17:05 by wuabdull         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	execute_builtin(t_command *cmd, t_shell *shell)
{
	if (cmd->arg_count == 0)
		return (ERROR);
	if (ft_strncmp(cmd->args[0].value, "echo", 5) == 0)
		return (ft_echo(cmd->args, cmd->arg_count));
	else if (ft_strncmp(cmd->args[0].value, "cd", 3) == 0)
		return (ft_cd(cmd->args, cmd->arg_count, shell));
	else if (ft_strncmp(cmd->args[0].value, "pwd", 4) == 0)
		return (ft_pwd());
	else if (ft_strncmp(cmd->args[0].value, "export", 7) == 0)
		return (ft_export(cmd->args, cmd->arg_count, shell));
	else if (ft_strncmp(cmd->args[0].value, "unset", 6) == 0)
		return (ft_unset(cmd->args, cmd->arg_count, shell));
	else if (ft_strncmp(cmd->args[0].value, "env", 4) == 0)
		return (ft_env(shell));
	else if (ft_strncmp(cmd->args[0].value, "exit", 5) == 0)
		return (ft_exit(cmd->args, cmd->arg_count, shell));
	return (ERROR);
}

int	handle_builtin_cmd(t_command *cmd, t_shell *shell,
		int stdin_backup, int stdout_backup)
{
	int	last_status;

	last_status = 0;
	if (cmd->pipe_fd[0] != -1)
		dup2(cmd->pipe_fd[0], STDIN_FILENO);
	if (cmd->pipe_fd[1] != -1)
		dup2(cmd->pipe_fd[1], STDOUT_FILENO);
	if (handle_redirections(cmd->redirects) != ERROR)
	{
		if (cmd->arg_count > 0)
			last_status = execute_builtin(cmd, shell);
		else
			last_status = 0;
	}
	else
		last_status = 1;
	if (cmd->pipe_fd[0] != -1)
		close(cmd->pipe_fd[0]);
	if (cmd->pipe_fd[1] != -1)
		close(cmd->pipe_fd[1]);
	dup2(stdin_backup, STDIN_FILENO);
	dup2(stdout_backup, STDOUT_FILENO);
	return (last_status);
}
