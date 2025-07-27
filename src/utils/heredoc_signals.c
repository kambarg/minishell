/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_heredoc.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkambarb <gkambarb@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 03:24:16 by gkambarb          #+#    #+#             */
/*   Updated: 2025/07/28 03:24:43 by gkambarb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	handle_line_exit(t_shell *shell, char *line, t_redirect *redir)
{
	int	tty_fd;

	if (g_exec_status == 3)
	{
		tty_fd = open("/dev/tty", O_RDONLY);
		if (tty_fd == -1)
		{
			perror("open /dev/tty");
			exit(EXIT_FAILURE);
		}
		if (tty_fd != STDIN_FILENO)
		{
			dup2(tty_fd, STDIN_FILENO);
			close(tty_fd);
		}
		shell->exit_status = 130;
		return (1);
	}
	if (!line \
|| ft_strncmp(line, redir->file, ft_strlen(redir->file) + 1) == 0)
		return (1);
	return (0);
}
