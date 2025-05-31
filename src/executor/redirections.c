#include "../../includes/minishell.h"

static int	handle_heredoc(char *delimiter)
{
	char	*line;
	char	temp_template[] = "/tmp/minishell_heredoc_XXXXXX";
	int		temp_fd;
	int		read_fd;

	/* Create unique temporary file */
	temp_fd = mkstemp(temp_template);
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
	print_heredoc_temp_files(temp_template);
	
	/* Reopen temp file for reading */
	read_fd = open(temp_template, O_RDONLY);
	if (read_fd == -1)
	{
		print_error("heredoc", strerror(errno));
		unlink(temp_template);  /* Clean up on error */
		return (ERROR);
	}
	
	/* Mark file for deletion (it will be deleted when last fd is closed) */
	unlink(temp_template);
	
	/* Redirect stdin to the temporary file */
	if (dup2(read_fd, STDIN_FILENO) == -1)
	{
		print_error("heredoc", strerror(errno));
		close(read_fd);
		return (ERROR);
	}
	
	close(read_fd);
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
			if (handle_heredoc(redirects->file) == ERROR)
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