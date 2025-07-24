/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkambarb <gkambarb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 14:54:43 by gkambarb          #+#    #+#             */
/*   Updated: 2025/07/16 14:54:43 by gkambarb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// Process heredoc using pipes - more efficient than temp files
static int	process_heredoc_pipe(t_redirect *redir, t_shell *shell)
{
	int		pipefd[2];
	char	*line;
	char	*expanded;

	if (pipe(pipefd) == -1)
	{
		print_error("heredoc", strerror(errno));
		return (ERROR);
	}
	setup_signals_heredoc();
	while (1)
	{
		line = readline("> ");
		if (!line || ft_strncmp(line, redir->file, ft_strlen(redir->file) + 1) == 0)
		{
			free(line);
			break ;
		}
		if (redir->quote_type == QUOTE_NONE)
		{
			expanded = expand_quoted_string(line, shell, QUOTE_NONE);
			if (expanded)
			{
				ft_putendl_fd(expanded, pipefd[1]);
				free(expanded);
			}
			else
				ft_putendl_fd(line, pipefd[1]);
		}
		else
			ft_putendl_fd(line, pipefd[1]);
		free(line);
	}
	setup_signals_interactive();
	close(pipefd[1]);  // Close write end
	redir->fd = pipefd[0];  // Store read end
	return (SUCCESS);
}

// Pre-process all heredocs in parent process before forking:
// 1) Create pipe and process heredoc content in parent
// 2) Store read file descriptor in redirect structure
// 3) Convert heredoc to input redirect
int	preprocess_heredocs(t_command *commands, t_shell *shell)
{
	t_command	*cmd;
	t_redirect	*redir;

	cmd = commands;
	while (cmd)
	{
		redir = cmd->redirects;
		while (redir)
		{
			if (redir->type == REDIR_HEREDOC)
			{
				if (process_heredoc_pipe(redir, shell) == ERROR)
					return (ERROR);
				redir->type = REDIR_IN;
			}
			redir = redir->next;
		}
		cmd = cmd->next;
	}
	return (SUCCESS);
}

// If temp_fd is provided (heredoc pipe fd), use it directly
// Don't close temp_fd, will be closed when command structure is freed
// Else regular file input redirect
static int	handle_input_redirect(char *file, int temp_fd)
{
	int	fd;

	if (temp_fd != -1)
	{
		if (dup2(temp_fd, STDIN_FILENO) == -1)
		{
			print_error("heredoc", strerror(errno));
			return (ERROR);
		}
		return (SUCCESS);
	}
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

// REDIR_HEREDOC should not happen anymore after preprocessing
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
