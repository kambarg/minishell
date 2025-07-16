/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkambarb <gkambarb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 16:33:49 by wuabdull          #+#    #+#             */
/*   Updated: 2025/07/16 13:16:05 by gkambarb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	is_builtin(char *cmd)
{
	return (ft_strncmp(cmd, "echo", 5) == 0
		|| ft_strncmp(cmd, "cd", 3) == 0
		|| ft_strncmp(cmd, "pwd", 4) == 0
		|| ft_strncmp(cmd, "export", 7) == 0
		|| ft_strncmp(cmd, "unset", 6) == 0
		|| ft_strncmp(cmd, "env", 4) == 0
		|| ft_strncmp(cmd, "exit", 5) == 0);
}

// If there was an external command, wait for it and use its status
// Otherwise, use the builtin command status
int	execute_commands(t_shell *shell)
{
	int		stdin_backup;
	int		stdout_backup;
	pid_t	last_pid;
	int		last_status;

	if (preprocess_heredocs(shell->commands, shell) == ERROR)
		return (ERROR);
	stdin_backup = dup(STDIN_FILENO);
	stdout_backup = dup(STDOUT_FILENO);
	init_pipe_fds_loop(shell->commands);
	setup_pipes_loop(shell->commands);
	last_status = run_command_loop(shell, stdin_backup,
			stdout_backup, &last_pid);
	if (last_pid != -1)
		return (wait_and_return_status(last_pid, stdin_backup, stdout_backup));
	close(stdin_backup);
	close(stdout_backup);
	return (last_status);
}
