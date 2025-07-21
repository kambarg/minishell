/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_external.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wuabdull <wuabdull@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 16:49:57 by wuabdull          #+#    #+#             */
/*   Updated: 2025/07/21 20:29:19 by wuabdull         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	exec_external_cmd(t_command *cmd, t_shell *shell)
{
	char	**argv;
	char	*cmd_path;

	argv = create_argv(cmd);
	cmd_path = find_command_path(cmd->args[0].value, shell->env);
	if (!cmd_path)
	{
		print_error(cmd->args[0].value, "command not found");
		free(argv);
		exit(127);
	}
	reset_signals_default();
	execve(cmd_path, argv, shell->env);
	print_error(cmd->args[0].value, strerror(errno));
	free(cmd_path);
	free(argv);
	exit(126);
}

void	run_external_child(t_command *cmd, t_shell *shell)
{
	if (cmd->pipe_fd[0] != -1)
		dup2(cmd->pipe_fd[0], STDIN_FILENO);
	if (cmd->pipe_fd[1] != -1)
		dup2(cmd->pipe_fd[1], STDOUT_FILENO);
	close_all_pipes(shell->commands);
	if (handle_redirections(cmd->redirects) == ERROR)
		exit(1);
	if (cmd->arg_count > 0 && is_builtin(cmd->args[0].value))
		exit(execute_builtin(cmd, shell));
	if (cmd->arg_count > 0)
		exec_external_cmd(cmd, shell);
	exit(0);
}

int	handle_external_cmd(t_command *cmd, t_shell *shell, int *last_pid)
{
	pid_t	pid;

	if (cmd->args && cmd->arg_count > 0 && cmd->args[0].value[0] == '\0')
	{
		ft_putstr_fd("minishell: : command not found\n", STDERR_FILENO);
		return (127);
	}
	pid = fork();
	if (pid == -1)
	{
		print_error("fork", strerror(errno));
		return (ERROR);
	}
	if (pid == 0)
		run_external_child(cmd, shell);
	*last_pid = pid;
	if (cmd->pipe_fd[0] != -1)
		close(cmd->pipe_fd[0]);
	if (cmd->pipe_fd[1] != -1)
		close(cmd->pipe_fd[1]);
	return (0);
}
