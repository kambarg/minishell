/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   temp_files.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkambarb <gkambarb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 15:17:36 by gkambarb          #+#    #+#             */
/*   Updated: 2025/07/16 15:17:38 by gkambarb         ###   ########.fr       */
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

static void	write_heredoc_loop(int write_fd, char *delimiter)
{
	char	*line;
	int		del_len;

	del_len = ft_strlen(delimiter);
	while (1)
	{
		line = readline("> ");
		if (!line || ft_strncmp(line, delimiter, del_len + 1) == 0)
		{
			free(line);
			break ;
		}
		ft_putendl_fd(line, write_fd);
		free(line);
	}
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

int	create_temp_file(char *delimiter, int *temp_fd, t_shell *shell)
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
	write_heredoc_loop(write_fd, delimiter);
	setup_signals_interactive();
	close(write_fd);
	status = handle_heredoc_file(temp_path, temp_fd);
	return (status);
}
