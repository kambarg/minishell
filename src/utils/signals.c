/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wuabdull <wuabdull@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 14:05:46 by wuabdull          #+#    #+#             */
/*   Updated: 2025/07/16 14:05:46 by wuabdull         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	g_exec_status = 0;

void	handle_signals(int signum)
{
	printf("handle_signals\n");
	if (signum == SIGINT)
	{
		if (g_exec_status == 0)
		{
			write(STDOUT_FILENO, "\n", 1);
			rl_replace_line("", 0);
			rl_on_new_line();
			rl_redisplay();
		}
		else if (g_exec_status == 2)
		{
			write(STDOUT_FILENO, "\n", 1);
			close(STDIN_FILENO);
			g_exec_status = 0;
		}
		//g_exec_status = 3;
	}
}

void	setup_signals_interactive(void)
{
	printf("setup_signals_interactive\n");
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
