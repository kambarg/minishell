/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkambarb <gkambarb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 15:49:25 by wuabdull          #+#    #+#             */
/*   Updated: 2025/07/25 01:08:10 by gkambarb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	init_heredoc_pipe(int pipefd[2])
{
	if (pipe(pipefd) == -1)
	{
		print_error("heredoc", strerror(errno));
		return (0);
	}
	return (1);
}

static int	should_stop_heredoc(char *line, const char *delimiter)
{
	if (!line)
		return (1);
	if (ft_strncmp(line, delimiter, ft_strlen(delimiter) + 1) == 0)
		return (1);
	return (0);
}

static void	handle_line(char *line, t_redirect *redir,\
	t_shell *shell, int write_fd)
{
	char	*expanded;

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
}

int	process_heredoc_pipe(t_redirect *redir, t_shell *shell)
{
	int		pipefd[2];
	char	*line;

	if (!init_heredoc_pipe(pipefd))
		return (ERROR);
	setup_signals_heredoc();
	while (1)
	{
		line = readline("> ");
		if (should_stop_heredoc(line, redir->file))
		{
			free(line);
			break ;
		}
		handle_line(line, redir, shell, pipefd[1]);
		free(line);
	}
	setup_signals_interactive();
	close(pipefd[1]);
	redir->fd = pipefd[0];
	return (SUCCESS);
}

void	setup_pipes(t_command *cmd)
{
	int	pipefd[2];

	if (!cmd->next)
		return ;
	if (pipe(pipefd) == -1)
	{
		print_error("pipe", strerror(errno));
		return ;
	}
	cmd->pipe_fd[1] = pipefd[1];
	cmd->next->pipe_fd[0] = pipefd[0];
}
