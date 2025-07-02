#include "../../includes/minishell.h"

/* Create a unique temporary file path using pid and shell counter */
char	*create_unique_temp_path(t_shell *shell)
{
	char	*temp_path;
	char	*pid_str;
	char	*counter_str;
	char	*temp1;
	char	*temp2;
	int		fd;

	if (!shell)
		return (NULL);

	/* Get process ID and convert to string */
	pid_str = ft_itoa(getpid());
	if (!pid_str)
		return (NULL);
	
	/* Try to create unique file */
	while (1)
	{
		/* Convert counter to string */
		counter_str = ft_itoa(shell->temp_file_counter++);
		if (!counter_str)
		{
			free(pid_str);
			return (NULL);
		}
		
		/* Build temp file path: /tmp/minishell_heredoc_<pid>_<counter> */
		temp1 = ft_strjoin("/tmp/minishell_heredoc_", pid_str);
		temp2 = ft_strjoin(temp1, "_");
		free(temp1);
		temp_path = ft_strjoin(temp2, counter_str);
		free(temp2);
		free(counter_str);
		
		if (!temp_path)
		{
			free(pid_str);
			return (NULL);
		}
		
		/* Try to create file with O_EXCL to ensure uniqueness */
		fd = open(temp_path, O_CREAT | O_EXCL | O_RDWR, 0600);
		if (fd != -1)
		{
			close(fd);
			free(pid_str);
			return (temp_path);
		}
		
		/* File exists, try next counter value */
		free(temp_path);
	}
}

/* Add a temp file path to the shell's temp file list */
void	add_temp_file(t_shell *shell, char *path)
{
	t_temp_file	*new_node;

	if (!shell || !path)
		return ;
		
	new_node = malloc(sizeof(t_temp_file));
	if (!new_node)
		return ;
		
	new_node->path = ft_strdup(path);
	if (!new_node->path)
	{
		free(new_node);
		return ;
	}
	
	new_node->next = shell->temp_files;
	shell->temp_files = new_node;
}

/* Clean up all temporary files */
void	cleanup_temp_files(t_shell *shell)
{
	t_temp_file	*current;
	t_temp_file	*next;

	if (!shell)
		return ;
		
	current = shell->temp_files;
	while (current)
	{
		next = current->next;
		
		/* Remove the actual file */
		unlink(current->path);
		
		/* Free the memory */
		free(current->path);
		free(current);
		
		current = next;
	}
	
	shell->temp_files = NULL;
} 