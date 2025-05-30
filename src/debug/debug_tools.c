// #######################################################
// These tools are used to debug and are not the part 
// of the final minishell project to submit
// #######################################################

#include "../../includes/minishell.h"

// Helper function to convert token type to string
static char	*get_token_type_str(int type)
{
	if (type == T_WORD)
		return ("T_WORD");
	else if (type == T_PIPE)
		return ("T_PIPE");
	else if (type == T_REDIR_IN)
		return ("T_REDIR_IN");
	else if (type == T_REDIR_OUT)
		return ("T_REDIR_OUT");
	else if (type == T_HEREDOC)
		return ("T_HEREDOC");
	else if (type == T_APPEND)
		return ("T_APPEND");
	else
		return ("T_UNKNOWN");
}

// Print tokens lexer fucntion created from the input line
void	print_tokens(t_token *tokens)
{
	t_token	*current;
	int fd;
	int count;

	fd = open("debug.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1) 
	{
    	printf ("Failed to open debug.txt file\n");
		return ;
    }

	if (!tokens)
	{
		ft_putstr_fd("No tokens to display.\n", fd);
		close(fd);
		return ;
	}
	
	current = tokens;
	count = 1;
	ft_putstr_fd("Resulting Token List:\n", fd);
	while (current)
	{
		ft_putnbr_fd(count, fd);
		ft_putstr_fd(". \"", fd);
		ft_putstr_fd(current->value, fd);
		ft_putstr_fd("\" (", fd);
		ft_putstr_fd(get_token_type_str(current->type), fd);
		ft_putstr_fd(")\n", fd);
		current = current->next;
		count++;
	}
	close(fd);
}
