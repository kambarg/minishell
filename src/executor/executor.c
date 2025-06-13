#include "../../includes/minishell.h"

int	is_builtin(char *cmd)
{
	return (ft_strncmp(cmd, "echo", 5) == 0 || ft_strncmp(cmd, "cd", 3) == 0 ||
			ft_strncmp(cmd, "pwd", 4) == 0 || ft_strncmp(cmd, "export", 7) == 0 ||
			ft_strncmp(cmd, "unset", 6) == 0 || ft_strncmp(cmd, "env", 4) == 0 ||
			ft_strncmp(cmd, "exit", 5) == 0);
}

int	execute_builtin(t_command *cmd, t_shell *shell)
{
	if (ft_strncmp(cmd->args[0], "echo", 5) == 0)
		return (ft_echo(cmd->args));
	else if (ft_strncmp(cmd->args[0], "cd", 3) == 0)
		return (ft_cd(cmd->args, shell));
	else if (ft_strncmp(cmd->args[0], "pwd", 4) == 0)
		return (ft_pwd());
	else if (ft_strncmp(cmd->args[0], "export", 7) == 0)
		return (ft_export(cmd->args, shell));
	else if (ft_strncmp(cmd->args[0], "unset", 6) == 0)
		return (ft_unset(cmd->args, shell));
	else if (ft_strncmp(cmd->args[0], "env", 4) == 0)
		return (ft_env(shell));
	else if (ft_strncmp(cmd->args[0], "exit", 5) == 0)
		return (ft_exit(cmd->args, shell));
	return (ERROR);
}

char	*find_command_path(char *cmd, char **env)
{
	char	*path;
	char	*dir;
	char	*full_path;
	char	**paths;
	int		i;

	if (ft_strchr(cmd, '/'))
		return (ft_strdup(cmd));
	path = get_env_value(env, "PATH");
	if (!path)
		return (NULL);
	paths = ft_split(path, ':');
	free(path);
	i = 0;
	while (paths[i])
	{
		dir = ft_strjoin(paths[i], "/");
		full_path = ft_strjoin(dir, cmd);
		free(dir);
		if (access(full_path, X_OK) == 0)
		{
			free_array(paths);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	free_array(paths);
	return (NULL);
}

int	execute_commands(t_shell *shell)
{
	t_command	*cmd;
	int			last_status;
	int			stdin_backup;
	int			stdout_backup;
	pid_t		pid;
	int			status;
	pid_t		last_pid;

	/* Pre-process all heredocs in parent process before forking */
	if (preprocess_heredocs(shell->commands) == ERROR)
		return (ERROR);

	/* Backup original stdin and stdout */
	stdin_backup = dup(STDIN_FILENO);
	stdout_backup = dup(STDOUT_FILENO);
	
	/* Initialize pipe file descriptors for all commands */
	cmd = shell->commands;
	while (cmd)
	{
		cmd->pipe_fd[0] = -1;
		cmd->pipe_fd[1] = -1;
		cmd = cmd->next;
	}
	
	/* Set up pipes between commands */
	cmd = shell->commands;
	while (cmd)
	{
		setup_pipes(cmd);
		cmd = cmd->next;
	}
	
	/* Execute each command */
	cmd = shell->commands;
	last_pid = -1;
	while (cmd)
	{
		/* For built-in commands, execute directly only if it's a single command or the last one */
		if (cmd->args && cmd->args[0] && is_builtin(cmd->args[0]) && (!cmd->next || shell->commands == cmd))
		{
			/* Set up stdin/stdout redirections for pipes */
			if (cmd->pipe_fd[0] != -1)
				dup2(cmd->pipe_fd[0], STDIN_FILENO);
			if (cmd->pipe_fd[1] != -1)
				dup2(cmd->pipe_fd[1], STDOUT_FILENO);
			
			/* Handle redirections */
			if (handle_redirections(cmd->redirects) != ERROR)
			{
				/* Only execute builtin if there are arguments */
				if (cmd->args && cmd->args[0])
					last_status = execute_builtin(cmd, shell);
				else
					last_status = 0;  // Success for empty command with successful redirections
			}
			else
				last_status = 1;
			
			/* Close pipe file descriptors */
			if (cmd->pipe_fd[0] != -1)
				close(cmd->pipe_fd[0]);
			if (cmd->pipe_fd[1] != -1)
				close(cmd->pipe_fd[1]);
			
			/* Restore stdin and stdout */
			dup2(stdin_backup, STDIN_FILENO);
			dup2(stdout_backup, STDOUT_FILENO);
		}
		/* For external commands or builtins in a pipeline, fork and execute */
		else
		{
			pid = fork();
			if (pid == -1)
			{
				print_error("fork", strerror(errno));
				return (ERROR);
			}
			
			/* Child process */
			if (pid == 0)
			{
				/* Set up stdin/stdout redirections for pipes */
				if (cmd->pipe_fd[0] != -1)
					dup2(cmd->pipe_fd[0], STDIN_FILENO);
				if (cmd->pipe_fd[1] != -1)
					dup2(cmd->pipe_fd[1], STDOUT_FILENO);
				
				/* Close all pipe file descriptors in the child process */
				t_command *tmp = shell->commands;
				while (tmp)
				{
					if (tmp->pipe_fd[0] != -1)
						close(tmp->pipe_fd[0]);
					if (tmp->pipe_fd[1] != -1)
						close(tmp->pipe_fd[1]);
					tmp = tmp->next;
				}
				
				/* Handle redirections */
				if (handle_redirections(cmd->redirects) == ERROR)
					exit(1);
				
				/* Only execute command if there are arguments */
				if (cmd->args && cmd->args[0])
				{
					/* Execute builtin in child process if it's in a pipeline */
					if (is_builtin(cmd->args[0]))
					{
						exit(execute_builtin(cmd, shell));
					}
					else
					{
						/* Execute the external command */
						char *cmd_path = find_command_path(cmd->args[0], shell->env);
						if (!cmd_path)
						{
							print_error(cmd->args[0], "command not found");
							exit(127);
						}
						
						reset_signals_default();
						execve(cmd_path, cmd->args, shell->env);
						
						/* If execve fails */
						print_error(cmd->args[0], strerror(errno));
						free(cmd_path);
						exit(126);
					}
				}
				else
				{
					/* Empty command with successful redirections */
					exit(0);
				}
			}
			
			/* Parent process */
			last_pid = pid;
			
			/* Close pipe file descriptors for this command in parent */
			if (cmd->pipe_fd[0] != -1)
				close(cmd->pipe_fd[0]);
			if (cmd->pipe_fd[1] != -1)
				close(cmd->pipe_fd[1]);
		}
		
		cmd = cmd->next;
	}
	
	/* Wait for all child processes to complete */
	last_status = 0;

	/* Wait for the last command specifically to get its exit status */
	if (last_pid != -1)
	{
		waitpid(last_pid, &status, 0);
		
		/* Manual implementation of status checking without macros */
		if ((status & 0x7f) == 0) /* Process exited normally (equivalent to WIFEXITED) */
		{
			last_status = (status >> 8) & 0xff; /* Extract exit code (equivalent to WEXITSTATUS) */
		}
		else if (((status & 0x7f) + 1) >> 1 > 0) /* Process terminated by signal (equivalent to WIFSIGNALED) */
		{
			last_status = 128 + (status & 0x7f); /* 128 + signal number (equivalent to WTERMSIG) */
		}
			
		/* Then wait for any other remaining children */
		while (waitpid(-1, NULL, 0) > 0)
			;
	}
	
	/* Close backup file descriptors */
	close(stdin_backup);
	close(stdout_backup);
	
	return (last_status);
} 