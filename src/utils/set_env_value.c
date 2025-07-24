/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_env_value.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkambarb <gkambarb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 10:35:55 by gkambarb          #+#    #+#             */
/*   Updated: 2025/07/23 18:44:12 by gkambarb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	get_env_index(char **env, const char *name)
{
	int		i;
	size_t	len;

	i = 0;
	len = ft_strlen(name);
	while (env[i])
	{
		if ((ft_strncmp(env[i], name, len) == 0 && env[i][len] == '=')
			|| (ft_strncmp(env[i], name, len) == 0 && env[i][len] == '\0'
				&& ft_strlen(env[i]) == len))
			return (i);
		i++;
	}
	return (-1);
}

static int	update_env_entry(t_shell *shell, const char *name,
		const char *value, int i)
{
	char	*new_entry;
	char	*temp;

	free(shell->env[i]);
	if (value)
	{
		temp = ft_strjoin(name, "=");
		if (!temp)
			return (0);
		new_entry = ft_strjoin(temp, value);
		free(temp);
	}
	else
	{
		new_entry = ft_strdup(name);
	}
	if (!new_entry)
		return (0);
	shell->env[i] = new_entry;
	return (1);
}

static int	is_invalid_name(const char *name)
{
	if (!name || !*name || ft_strchr(name, '='))
		return (1);
	return (0);
}

void	set_env_value(t_shell *shell, const char *name, const char *value)
{
	int	i;
	int	result;

	if (!shell || !shell->env || is_invalid_name(name))
		return ;
	i = get_env_index(shell->env, name);
	if (i >= 0)
	{
		result = update_env_entry(shell, name, value, i);
		if (!result)
			print_error("export", "memory allocation failed");
	}
	else
	{
		result = append_env_entry(shell, name, value);
		if (!result)
			print_error("export", "memory allocation failed");
	}
}

// Note: if variable already exists, don't change its value
void	set_env_no_value(t_shell *shell, const char *name)
{
	int	i;
	int	result;

	if (!shell || !shell->env || is_invalid_name(name))
		return ;
	i = get_env_index(shell->env, name);
	if (i >= 0)
		return ;
	else
	{
		result = append_env_entry(shell, name, NULL);
		if (!result)
			print_error("export", "memory allocation failed");
	}
}
