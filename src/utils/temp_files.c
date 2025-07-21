/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   temp_files.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkambarb <gkambarb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 15:17:36 by gkambarb          #+#    #+#             */
/*   Updated: 2025/07/19 22:27:37 by gkambarb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	open_write_fd(char *temp_path)
{
	int	fd;

	fd = open(temp_path, O_WRONLY);
	if (fd == -1)
		print_error("heredoc", strerror(errno));
	return (fd);
}

static int	open_read_fd(char *temp_path)
{
	int	fd;

	fd = open(temp_path, O_RDONLY);
	if (fd == -1)
		print_error("heredoc", strerror(errno));
	return (fd);
}

// Note: expand variables in heredoc text only if delimiter was not quoted
static void	write_heredoc_loop(int write_fd, t_redirect *redir, t_shell *shell)
{
	char	*line;
	char	*expanded;

	while (1)
	{
		line = readline("> ");
		if (!line \
|| ft_strncmp(line, redir->file, ft_strlen(redir->file) + 1) == 0)
			break ;
		if (redir->quote_type == QUOTE_NONE)
		{
			expanded = expand_quoted_string(line, shell, QUOTE_NONE);
			if (expanded)
			{
				ft_putendl_fd(expanded, write_fd);
				free(expanded);
			}
			else
				ft_putendl_fd(line, write_fd);
		}
		else
			ft_putendl_fd(line, write_fd);
		free(line);
	}
	free(line);
}

static int	handle_heredoc_file(char *temp_path, int *temp_fd)
{
	int	read_fd;

	read_fd = open_read_fd(temp_path);
	if (read_fd == -1)
	{
		unlink(temp_path);
		free(temp_path);
		return (ERROR);
	}
	unlink(temp_path);
	free(temp_path);
	*temp_fd = read_fd;
	return (SUCCESS);
}

int	create_temp_file(t_redirect *redir, int *temp_fd, t_shell *shell)
{
	char	*temp_path;
	int		write_fd;
	int		status;

	temp_path = create_temp_path(shell);
	if (!temp_path)
	{
		print_error("heredoc", "failed to create temp file");
		return (ERROR);
	}
	write_fd = open_write_fd(temp_path);
	if (write_fd == -1)
		return (free(temp_path), ERROR);
	setup_signals_heredoc();
	write_heredoc_loop(write_fd, redir, shell);
	setup_signals_interactive();
	close(write_fd);
	status = handle_heredoc_file(temp_path, temp_fd);
	return (status);
}
