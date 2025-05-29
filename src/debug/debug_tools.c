// #######################################################
// These tools are used to debug and are not the part 
// of the final minishell project to submit
// #######################################################

#include "../../includes/minishell.h"

// Print tokens lexer fucntion created from the input line
void	print_tokens(t_token *tokens)
{
	t_token	*current;
	int fd;

	fd = open("debug.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1) 
	{
    	printf ("Failed to open debug.txt file\n");
    }

	if (!tokens)
	{
		ft_putstr_fd("No tokens to display.\n", fd);
		return ;
	}
	current = tokens;
	ft_putstr_fd("!!! Debug printing:\n", fd);
	while (current)
	{
		ft_putstr_fd("Token: ", fd);
		ft_putstr_fd(current->value, fd);
		//ft_putstr_fd(" Type: ", fd);
		//ft_putnbr_fd(current->type, fd);
		ft_putchar_fd('\n', fd);
		current = current->next;
	}
	ft_putstr_fd("!!! Debug printing:\n", fd);
}
