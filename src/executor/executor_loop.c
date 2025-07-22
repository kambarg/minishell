/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_loop.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkambarb <gkambarb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 16:50:21 by wuabdull          #+#    #+#             */
/*   Updated: 2025/07/22 20:52:39 by gkambarb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	run_command_loop(t_shell *shell, int stdin_backup, int stdout_backup,
		pid_t *last_pid)
{
	t_command	*cmd;
	int			last_status;

	cmd = shell->commands;
	*last_pid = -1;
	last_status = 0;
	while (cmd)
	{
		if (cmd->arg_count == 0 || !cmd->args[0].value
			|| cmd->args[0].value[0] == '\0')
		{
			ft_putstr_fd("minishell: : command not found\n", STDERR_FILENO);
			last_status = 127;
			cmd = cmd->next;
			continue ;
		}
		if (is_builtin(cmd->args[0].value) && (!cmd->next
				|| shell->commands == cmd))
			last_status = handle_builtin_cmd(cmd, shell, stdin_backup,
					stdout_backup);
		else
			handle_external_cmd(cmd, shell, last_pid);
		cmd = cmd->next;
	}
	return (last_status);
}

int	wait_and_return_status(pid_t last_pid, int stdin_backup, int stdout_backup)
{
	int	last_status;
	int	status;

	last_status = 0;
	if (last_pid != -1)
	{
		waitpid(last_pid, &status, 0);
		if ((status & 0x7f) == 0)
			last_status = (status >> 8) & 0xff;
		else if (((status & 0x7f) + 1) >> 1 > 0)
			last_status = 128 + (status & 0x7f);
		while (waitpid(-1, NULL, 0) > 0)
			;
	}
	close(stdin_backup);
	close(stdout_backup);
	return (last_status);
}
