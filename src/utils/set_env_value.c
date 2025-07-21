/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_env_value.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkambarb <gkambarb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 10:35:55 by gkambarb          #+#    #+#             */
/*   Updated: 2025/07/18 10:17:09 by gkambarb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	get_env_index(char **env, const char *name)
{
	int	i;
	int	len;

	i = 0;
	len = ft_strlen(name);
	while (env[i])
	{
		if (ft_strncmp(env[i], name, len) == 0 && env[i][len] == '=')
			return (i);
		i++;
	}
	return (-1);
}

static int	is_invalid(t_shell *shell, const char *name, const char *value)
{
	if (!shell || !name || !value || !shell->env)
		return (1);
	if (!*name || ft_strchr(name, '='))
		return (1);
	return (0);
}

int	update_env(t_shell *shell, const char *name, const char *value, int i)
{
	char	*new_var;
	char	*new_entry;

	free(shell->env[i]);
	new_var = ft_strjoin(name, "=");
	if (!new_var)
		return (0);
	new_entry = ft_strjoin(new_var, value);
	free(new_var);
	if (!new_entry)
		return (0);
	shell->env[i] = new_entry;
	return (1);
}

static int	append_new_env(t_shell *shell, const char *name, const char *value)
{
	int		env_size;
	char	**new_env;
	char	*new_var;
	int		i;

	env_size = 0;
	while (shell->env[env_size])
		env_size++;
	new_env = (char **)malloc(sizeof(char *) * (env_size + 2));
	if (!new_env)
		return (0);
	i = -1;
	while (++i < env_size)
		new_env[i] = shell->env[i];
	new_var = ft_strjoin(name, "=");
	if (!new_var)
		return (free(new_env), 0);
	new_env[i] = ft_strjoin(new_var, value);
	free(new_var);
	if (!new_env[i])
		return (free(new_env), 0);
	new_env[i + 1] = NULL;
	free(shell->env);
	shell->env = new_env;
	return (1);
}

void	set_env_value(t_shell *shell, const char *name, const char *value)
{
	int	i;
	int	result;

	if (is_invalid(shell, name, value))
		return ;
	i = get_env_index(shell->env, name);
	if (i >= 0)
	{
		result = update_env(shell, name, value, i);
		if (!result)
			print_error("export", "memory allocation failed");
	}
	else
	{
		result = append_new_env(shell, name, value);
		if (!result)
			print_error("export", "memory allocation failed");
	}
}
