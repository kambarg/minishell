/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wuabdull <wuabdull@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 13:29:39 by wuabdull          #+#    #+#             */
/*   Updated: 2025/07/14 14:26:32 by wuabdull         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	get_env_size(char **env)
{
	int	size;

	size = 0;
	while (env[size])
		size++;
	return (size);
}

void	bubble_sort_env(char **env, int size)
{
	int		i;
	int		j;
	char	*tmp;

	i = 0;
	while (i < size - 1)
	{
		j = 0;
		while (j < size - i - 1)
		{
			if (ft_strncmp(env[j], env[j + 1], ft_strlen(env[j]) + 1) > 0)
			{
				tmp = env[j];
				env[j] = env[j + 1];
				env[j + 1] = tmp;
			}
			j++;
		}
		i++;
	}
}

void	print_env_lines(char **env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		ft_putstr_fd("declare -x ", STDOUT_FILENO);
		ft_putendl_fd(env[i], STDOUT_FILENO);
		i++;
	}
}

void	print_sorted_env(char **env)
{
	int	size;

	size = get_env_size(env);
	bubble_sort_env(env, size);
	print_env_lines(env);
}
