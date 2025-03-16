#include "../../includes/minishell.h"

/* Create a pipe between two commands */
void	setup_pipes(t_command *cmd)
{
	int	pipefd[2];

	/* No need to set up pipes for the last command */
	if (!cmd->next)
		return;

	/* Create the pipe */
	if (pipe(pipefd) == -1)
	{
		print_error("pipe", strerror(errno));
		return;
	}

	/* Set the write end for the current command */
	cmd->pipe_fd[1] = pipefd[1];
	
	/* Set the read end for the next command */
	cmd->next->pipe_fd[0] = pipefd[0];
} 