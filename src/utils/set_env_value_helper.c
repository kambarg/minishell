/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_env_value_helper.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkambarb <gkambarb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 18:17:15 by gkambarb          #+#    #+#             */
/*   Updated: 2025/07/23 18:35:43 by gkambarb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	**allocate_env_copy(char **old_env, int size)
{
	char	**new_env;
	int		i;

	new_env = (char **)malloc(sizeof(char *) * (size + 2));
	if (!new_env)
		return (NULL);
	i = -1;
	while (++i < size)
		new_env[i] = old_env[i];
	return (new_env);
}

static char	*create_env_entry(const char *name, const char *value)
{
	char	*temp;
	char	*entry;

	if (value)
	{
		temp = ft_strjoin(name, "=");
		if (!temp)
			return (NULL);
		entry = ft_strjoin(temp, value);
		free(temp);
	}
	else
		entry = ft_strdup(name);
	return (entry);
}

int	append_env_entry(t_shell *shell, const char *name, const char *value)
{
	int		env_size;
	char	**new_env;
	char	*new_entry;

	env_size = get_env_size(shell->env);
	new_env = allocate_env_copy(shell->env, env_size);
	if (!new_env)
		return (0);
	new_entry = create_env_entry(name, value);
	if (!new_entry)
		return (free(new_env), 0);
	new_env[env_size] = new_entry;
	new_env[env_size + 1] = NULL;
	free(shell->env);
	shell->env = new_env;
	return (1);
}
