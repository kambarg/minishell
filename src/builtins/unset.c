/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+
	+:+     */
/*   By: wuabdull <wuabdull@student.42.fr>          +#+  +:+
	+#+        */
/*                                                +#+#+#+#+#+
	+#+           */
/*   Created: 2025/07/14 14:29:43 by wuabdull          #+#    #+#             */
/*   Updated: 2025/07/18 11:40:25 by gkambarb         ###   ########.fr       */
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

static int	var_exists_in_env(t_shell *shell, const char *name, size_t len)
{
	int	i;

	i = 0;
	while (shell->env[i])
	{
		if (ft_strncmp(shell->env[i], name, len) == 0
			&& (shell->env[i][len] == '=' || (shell->env[i][len] == '\0'
			&& ft_strlen(shell->env[i]) == len)))
			return (1);
		i++;
	}
	return (0);
}

static void	copy_env_except_name(t_shell *shell, char **new_env,
		const char *name, size_t len)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (shell->env[i])
	{
		if (ft_strncmp(shell->env[i], name, len) != 0
			|| (shell->env[i][len] != '=' && !(shell->env[i][len] == '\0'
			&& ft_strlen(shell->env[i]) == len)))
		{
			new_env[j] = ft_strdup(shell->env[i]);
			if (!new_env[j])
			{
				while (--j >= 0)
					free(new_env[j]);
				free(new_env);
				return ;
			}
			j++;
		}
		i++;
	}
	new_env[j] = NULL;
}

static void	remove_env_var(t_shell *shell, const char *name)
{
	size_t	len;
	char	**new_env;
	int		env_size;

	if (!name || !*name)
		return ;
	len = ft_strlen(name);
	if (!var_exists_in_env(shell, name, len))
		return ;
	env_size = get_env_size_for_remove(shell);
	new_env = (char **)malloc(sizeof(char *) * (env_size + 1));
	if (!new_env)
		return ;
	copy_env_except_name(shell, new_env, name, len);
	if (new_env)
	{
		free_array(shell->env);
		shell->env = new_env;
	}
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
