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
	ft_putstr_fd("=== Resulting Tokens List ===\n", fd);
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

// Debug function to print heredoc temporary file info
void	print_heredoc_temp_files(char *temp_path)
{
	int		debug_fd;
	int		temp_fd;
	char	buffer[1024];
	int		bytes_read;

	debug_fd = open("debug.txt", O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (debug_fd == -1)
	{
		printf("Failed to open debug.txt file\n");
		return ;
	}

	ft_putstr_fd("\n=== Heredoc temp file ===\n", debug_fd);
	ft_putstr_fd("Temp file path: ", debug_fd);
	ft_putstr_fd(temp_path, debug_fd);
	ft_putstr_fd("\n", debug_fd);

	// Try to open and read the temp file
	temp_fd = open(temp_path, O_RDONLY);
	if (temp_fd == -1)
	{
		ft_putstr_fd("Status: FILE NOT FOUND (possibly deleted)\n", debug_fd);
		ft_putstr_fd("================================\n\n", debug_fd);
		close(debug_fd);
		return ;
	}
	ft_putstr_fd("File content:\n", debug_fd);
	
	// Read and print file content
	while ((bytes_read = read(temp_fd, buffer, sizeof(buffer) - 1)) > 0)
	{
		buffer[bytes_read] = '\0';
		ft_putstr_fd(buffer, debug_fd);
	}
	ft_putstr_fd("\n", debug_fd);

	close(temp_fd);
	close(debug_fd);
}
