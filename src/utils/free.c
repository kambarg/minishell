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
		/* Close heredoc temp file descriptor if open */
		if (temp->fd != -1)
			close(temp->fd);
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