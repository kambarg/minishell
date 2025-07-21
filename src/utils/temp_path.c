/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   temp_path.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkambarb <gkambarb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 15:17:45 by gkambarb          #+#    #+#             */
/*   Updated: 2025/07/16 15:18:37 by gkambarb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*generate_pid_str(t_shell *shell)
{
	unsigned long	address;
	int				pid_int;
	char			*pid_str;

	address = (unsigned long)&shell;
	pid_int = (int)(address % 100000);
	pid_str = ft_itoa(pid_int);
	return (pid_str);
}

static char	*build_temp_path(char *pid_str, int counter)
{
	char	*counter_str;
	char	*temp1;
	char	*temp2;
	char	*final_path;

	counter_str = ft_itoa(counter);
	if (!counter_str)
		return (NULL);
	temp1 = ft_strjoin("/tmp/minishell_heredoc_", pid_str);
	if (!temp1)
		return (free(counter_str), NULL);
	temp2 = ft_strjoin(temp1, "_");
	free(temp1);
	if (!temp2)
		return (free(counter_str), NULL);
	final_path = ft_strjoin(temp2, counter_str);
	free(temp2);
	free(counter_str);
	return (final_path);
}

static char	*try_create_unique_file(t_shell *shell, char *pid_str)
{
	char	*temp_path;
	int		fd;

	while (1)
	{
		temp_path = build_temp_path(pid_str, shell->temp_file_counter++);
		if (!temp_path)
			return (NULL);
		fd = open(temp_path, O_CREAT | O_EXCL | O_RDWR, 0600);
		if (fd != -1)
		{
			close(fd);
			return (temp_path);
		}
		free(temp_path);
	}
}

char	*create_temp_path(t_shell *shell)
{
	char	*pid_str;
	char	*temp_path;

	if (!shell)
		return (NULL);
	pid_str = generate_pid_str(shell);
	if (!pid_str)
		return (NULL);
	temp_path = try_create_unique_file(shell, pid_str);
	free(pid_str);
	return (temp_path);
}
