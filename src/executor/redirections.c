#include "../../includes/minishell.h"

static int	create_heredoc_temp_file(char *delimiter, int *temp_fd, t_shell *shell)
{
	char	*line;
	char	*temp_path;
	int		write_fd;
	int		read_fd;

	/* Create temporary file path and write descriptor */
	temp_path = create_unique_temp_path(shell);
	if (!temp_path)
	{
		print_error("heredoc", "failed to create temp file");
		return (ERROR);
	}
	
	/* Open for writing */
	write_fd = open(temp_path, O_WRONLY);
	if (write_fd == -1)
	{
		print_error("heredoc", strerror(errno));
		free(temp_path);
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
		ft_putendl_fd(line, write_fd);
		free(line);
	}
	
	setup_signals_interactive();
	
	/* Close write descriptor */
	close(write_fd);
	
	/* Open for reading before unlinking */
	read_fd = open(temp_path, O_RDONLY);
	if (read_fd == -1)
	{
		print_error("heredoc", strerror(errno));
		unlink(temp_path);
		free(temp_path);
		return (ERROR);
	}
	
	/* Now unlink to make file invisible and secure */
	unlink(temp_path);
	free(temp_path);
	
	/* Return read file descriptor */
	*temp_fd = read_fd;
	
	/* DEBUG: Show temp file info */
	ft_putstr_fd("Secure heredoc temp file created (invisible in filesystem)\n", 2);
	
	return (SUCCESS);
}

/* Pre-process all heredocs in parent process before forking */
int	preprocess_heredocs(t_command *commands, t_shell *shell)
{
	t_command	*cmd;
	t_redirect	*redir;
	int			temp_fd;

	cmd = commands;
	while (cmd)
	{
		redir = cmd->redirects;
		while (redir)
		{
			if (redir->type == REDIR_HEREDOC)
			{
				/* Create secure temp file and process heredoc in parent */
				if (create_heredoc_temp_file(redir->file, &temp_fd, shell) == ERROR)
					return (ERROR);
				
				/* Store file descriptor in redirect structure */
				redir->fd = temp_fd;
				
				/* Keep delimiter in file field for reference but mark as processed */
				/* Convert heredoc to special heredoc input redirect */
				redir->type = REDIR_IN;
			}
			redir = redir->next;
		}
		cmd = cmd->next;
	}
	return (SUCCESS);
}

static int	handle_input_redirect(char *file, int temp_fd)
{
	int	fd;

	/* If temp_fd is provided (heredoc temp file), use it directly */
	if (temp_fd != -1)
	{
		if (dup2(temp_fd, STDIN_FILENO) == -1)
		{
			print_error("heredoc", strerror(errno));
			return (ERROR);
		}
		/* Note: Don't close temp_fd here, it will be closed when command structure is freed */
		return (SUCCESS);
	}
	
	/* Regular file input redirect */
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
			if (handle_input_redirect(redirects->file, redirects->fd) == ERROR)
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