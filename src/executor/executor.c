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
	while (cmd && cmd->next)
	{
		setup_pipes(cmd);
		cmd = cmd->next;
	}
	
	/* Execute each command */
	cmd = shell->commands;
	while (cmd)
	{
		/* Skip empty commands */
		if (!cmd->args || !cmd->args[0])
		{
			cmd = cmd->next;
			continue;
		}
		
		/* For built-in commands, execute directly */
		if (is_builtin(cmd->args[0]))
		{
			/* Set up stdin/stdout redirections for pipes */
			if (cmd->pipe_fd[0] != -1)
				dup2(cmd->pipe_fd[0], STDIN_FILENO);
			if (cmd->pipe_fd[1] != -1)
				dup2(cmd->pipe_fd[1], STDOUT_FILENO);
			
			/* Handle redirections */
			if (handle_redirections(cmd->redirects) != ERROR)
				last_status = execute_builtin(cmd, shell);
			
			/* Close pipe file descriptors */
			if (cmd->pipe_fd[0] != -1)
				close(cmd->pipe_fd[0]);
			if (cmd->pipe_fd[1] != -1)
				close(cmd->pipe_fd[1]);
			
			/* Restore stdin and stdout */
			dup2(stdin_backup, STDIN_FILENO);
			dup2(stdout_backup, STDOUT_FILENO);
		}
		/* For external commands, fork and execute */
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
				
				/* Close all pipe file descriptors */
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
				
				/* Execute the command */
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
			
			/* Parent process */
			/* Close pipe file descriptors for this command */
			if (cmd->pipe_fd[0] != -1)
				close(cmd->pipe_fd[0]);
			if (cmd->pipe_fd[1] != -1)
				close(cmd->pipe_fd[1]);
		}
		
		cmd = cmd->next;
	}
	
	/* Wait for all child processes to complete */
	while (waitpid(-1, &status, 0) > 0)
	{
		if (WIFEXITED(status))
			last_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			last_status = 128 + WTERMSIG(status);
	}
	
	/* Close backup file descriptors */
	close(stdin_backup);
	close(stdout_backup);
	
	return (last_status);
} 