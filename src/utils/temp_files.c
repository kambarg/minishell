/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   temp_files.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkambarb <gkambarb@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 03:19:54 by gkambarb          #+#    #+#             */
/*   Updated: 2025/07/29 12:23:49 by gkambarb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	write_heredoc_loop(int write_fd, t_redirect *redir, t_shell *shell)
{
	char	*line;
	char	*expanded;

	while (1)
	{
		line = readline("> ");
		if (handle_line_exit(shell, line, redir))
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

int	create_temp_file(t_redirect *redir, int *temp_fd, t_shell *shell)
{
	int	pipe_fd[2];

	if (pipe(pipe_fd) == -1)
	{
		print_error("heredoc", strerror(errno));
		return (ERROR);
	}
	setup_signals_heredoc();
	write_heredoc_loop(pipe_fd[1], redir, shell);
	setup_signals_interactive();
	close(pipe_fd[1]);
	if (shell->exit_status == 130)
	{
		close(pipe_fd[0]);
		return (130);
	}
	*temp_fd = pipe_fd[0];
	return (SUCCESS);
}
