#include "../../includes/minishell.h"

/* Global variables to track execution state and signals */
int	g_exec_status = 0;

/* Signal handler for interactive mode (waiting for input) */
void	handle_signals(int signum)
{
	if (signum == SIGINT)
	{
		if (g_exec_status == 0) /* Interactive mode */
		{
			write(STDOUT_FILENO, "\n", 1);
			rl_on_new_line();
			rl_replace_line("", 0);
			rl_redisplay();
		}
		else if (g_exec_status == 2) /* Heredoc mode */
		{
			write(STDOUT_FILENO, "\n", 1);
			close(STDIN_FILENO); /* Force heredoc to exit */
			g_exec_status = 0;
		}
		g_exec_status = 3; /* Signal received */
		/* In execution mode 1, do nothing - let the child process handle it */
	}
}

/* Setup signals for interactive mode */
void	setup_signals_interactive(void)
{
	g_exec_status = 0;
	signal(SIGINT, handle_signals);
	signal(SIGQUIT, SIG_IGN);
}

/* Setup signals for command execution */
void	setup_signals_executing(void)
{
	g_exec_status = 1;
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}

/* Setup signals for heredoc reading */
void	setup_signals_heredoc(void)
{
	g_exec_status = 2;
	signal(SIGINT, handle_signals);
	signal(SIGQUIT, SIG_IGN);
}

/* Reset signals to default behavior (for child processes) */
void	reset_signals_default(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
} 