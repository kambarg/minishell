#include "../../includes/minishell.h"

void	free_tokens(t_token *tokens)
{
	t_token	*temp;

	while (tokens)
	{
		temp = tokens;
		tokens = tokens->next;
		if (temp->value)
			free(temp->value);
		free(temp);
	}
}

void	free_redirects(t_redirect *redirects)
{
	t_redirect	*temp;

	while (redirects)
	{
		temp = redirects;
		redirects = redirects->next;
		if (temp->file)
			free(temp->file);
		free(temp);
	}
}

void	free_commands(t_command *commands)
{
	t_command	*temp;
	int			i;

	while (commands)
	{
		temp = commands;
		commands = commands->next;
		
		if (temp->args)
		{
			i = 0;
			while (temp->args[i])
			{
				free(temp->args[i]);
				i++;
			}
			free(temp->args);
		}
		
		if (temp->redirects)
			free_redirects(temp->redirects);
		
		/* Close any open pipe file descriptors */
		if (temp->pipe_fd[0] != -1)
			close(temp->pipe_fd[0]);
		if (temp->pipe_fd[1] != -1)
			close(temp->pipe_fd[1]);
		
		free(temp);
	}
}

void	free_array(char **array)
{
	int	i;

	if (!array)
		return;
		
	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
} 