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

	/* Get unique identifier using shell address (truncated to int) */
	pid_str = ft_itoa((int)((unsigned long)&shell % 100000));
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


