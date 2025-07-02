#include "../../includes/minishell.h"

static int	create_heredoc_temp_file(char *delimiter, char **temp_path, t_shell *shell)
{
	char	*line;
	int		temp_fd;

	/* Create unique temporary file using only allowed functions */
	*temp_path = create_unique_temp_path(shell);
	if (!*temp_path)
	{
		print_error("heredoc", "failed to create temp file");
		return (ERROR);
	}
	
	/* Add temp file to shell's tracking list for cleanup */
	add_temp_file(shell, *temp_path);
	
	/* Open the temp file for writing */
	temp_fd = open(*temp_path, O_WRONLY);
	if (temp_fd == -1)
	{
		print_error("heredoc", strerror(errno));
		return (ERROR);
	}
	
	setup_signals_heredoc();
	
	/* Write heredoc content to temporary file */
	while (1)
	{
		line = readline("> ");
		if (!line || ft_strncmp(line, delimiter, ft_strlen(delimiter) + 1) == 0)
		{
			free(line);
			break ;
		}
		ft_putendl_fd(line, temp_fd);
		free(line);
	}
	
	setup_signals_interactive();
	
	/* Close write end of temp file */
	close(temp_fd);
	
	/* DEBUG: Show temp file after writing content */
	print_heredoc_temp_files(*temp_path);
	
	return (SUCCESS);
}

/* Pre-process all heredocs in parent process before forking */
int	preprocess_heredocs(t_command *commands, t_shell *shell)
{
	t_command	*cmd;
	t_redirect	*redir;
	char		*temp_path;

	cmd = commands;
	while (cmd)
	{
		redir = cmd->redirects;
		while (redir)
		{
			if (redir->type == REDIR_HEREDOC)
			{
				/* Create temp file and process heredoc in parent */
				if (create_heredoc_temp_file(redir->file, &temp_path, shell) == ERROR)
					return (ERROR);
				
				/* Replace delimiter with temp file path */
				free(redir->file);
				redir->file = temp_path;
				
				/* Convert heredoc to regular file input redirect */
				redir->type = REDIR_IN;
			}
			redir = redir->next;
		}
		cmd = cmd->next;
	}
	return (SUCCESS);
}

static int	handle_input_redirect(char *file)
{
	int	fd;

	fd = open(file, O_RDONLY);
	if (fd == -1)
	{
		print_error(file, strerror(errno));
		return (ERROR);
	}
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		print_error(file, strerror(errno));
		close(fd);
		return (ERROR);
	}
	close(fd);
	return (SUCCESS);
}

static int	handle_output_redirect(char *file, int append)
{
	int	fd;
	int	flags;

	flags = O_WRONLY | O_CREAT;
	if (append)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;
	fd = open(file, flags, 0644);
	if (fd == -1)
	{
		print_error(file, strerror(errno));
		return (ERROR);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		print_error(file, strerror(errno));
		close(fd);
		return (ERROR);
	}
	close(fd);
	return (SUCCESS);
}

int	handle_redirections(t_redirect *redirects)
{
	while (redirects)
	{
		if (redirects->type == REDIR_IN)
		{
			if (handle_input_redirect(redirects->file) == ERROR)
				return (ERROR);
		}
		else if (redirects->type == REDIR_OUT)
		{
			if (handle_output_redirect(redirects->file, 0) == ERROR)
				return (ERROR);
		}
		else if (redirects->type == REDIR_HEREDOC)
		{
			/* This should not happen anymore after preprocessing */
			print_error("heredoc", "unexpected heredoc in child process");
			return (ERROR);
		}
		else if (redirects->type == REDIR_APPEND)
		{
			if (handle_output_redirect(redirects->file, 1) == ERROR)
				return (ERROR);
		}
		redirects = redirects->next;
	}
	return (SUCCESS);
} 