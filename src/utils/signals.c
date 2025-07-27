/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wuabdull <wuabdull@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 14:05:46 by wuabdull          #+#    #+#             */
/*   Updated: 2025/07/28 02:12:00 by gkambarb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	g_exec_status = 0;

void	handle_signals(int signum)
{
	if (signum == SIGINT)
	{
		if (g_exec_status == 0 || g_exec_status == 3)
		{
			write(STDOUT_FILENO, "\n", 1);
			rl_on_new_line();
			rl_replace_line("", 0);
			rl_redisplay();
			g_exec_status = 3;
		}
		else if (g_exec_status == 2)
		{
			close(STDIN_FILENO);
			g_exec_status = 3;
		}
		g_exec_status = 3;
	}
}

void	setup_signals_interactive(void)
{
	g_exec_status = 0;
	signal(SIGINT, handle_signals);
	signal(SIGQUIT, SIG_IGN);
}

void	setup_signals_executing(void)
{
	g_exec_status = 1;
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}

void	setup_signals_heredoc(void)
{
	g_exec_status = 2;
	signal(SIGINT, handle_signals);
	signal(SIGQUIT, SIG_IGN);
}

void	reset_signals_default(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}
