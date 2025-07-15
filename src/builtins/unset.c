/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wuabdull <wuabdull@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 14:29:43 by wuabdull          #+#    #+#             */
/*   Updated: 2025/07/14 14:29:43 by wuabdull         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	get_env_size_for_remove(t_shell *shell)
{
	int	env_size;

	env_size = 0;
	while (shell->env[env_size])
		env_size++;
	return (env_size);
}

static void	copy_env_except_name(
	t_shell *shell, char **new_env, const char *name, int len)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (shell->env[i])
	{
		if (ft_strncmp(shell->env[i], name, len) != 0
			|| shell->env[i][len] != '=')
		{
			new_env[j] = ft_strdup(shell->env[i]);
			j++;
		}
		i++;
	}
	new_env[j] = NULL;
}

static void	remove_env_var(t_shell *shell, const char *name)
{
	int		len;
	char	**new_env;
	int		env_size;

	len = ft_strlen(name);
	env_size = get_env_size_for_remove(shell);
	new_env = (char **)malloc(sizeof(char *) * env_size);
	if (!new_env)
		return ;
	copy_env_except_name(shell, new_env, name, len);
	free_array(shell->env);
	shell->env = new_env;
}

int	ft_unset(t_arg_info *args, int arg_count, t_shell *shell)
{
	int	i;

	i = 1;
	while (i < arg_count)
	{
		remove_env_var(shell, args[i].value);
		i++;
	}
	return (SUCCESS);
}
