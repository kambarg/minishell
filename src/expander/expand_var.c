/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_var.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkambarb <gkambarb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 14:10:06 by gkambarb          #+#    #+#             */
/*   Updated: 2025/07/15 14:11:19 by gkambarb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*get_var_name(char *str, int *i)
{
	int		start;
	int		len;
	char	*name;

	if (!str || !i)
		return (NULL);
	start = *i;
	(*i)++;
	if (str[*i] == '?')
		return ((*i)++, ft_strdup("?"));
	if (str[*i] == '0')
		return ((*i)++, ft_strdup("0"));
	while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_'))
		(*i)++;
	len = *i - start - 1;
	if (len == 0)
		return (NULL);
	name = (char *)malloc(sizeof(char) * (len + 1));
	if (!name)
		return (NULL);
	ft_strlcpy(name, str + start + 1, len + 1);
	return (name);
}

char	*expand_var(char *str, t_shell *shell, int *i)
{
	char	*var_name;
	char	*result;

	if (!str || !shell || !i)
		return (ft_strdup("$"));
	var_name = get_var_name(str, i);
	if (!var_name)
		return (ft_strdup("$"));
	if (ft_strlen(var_name) == 1 && ft_strncmp(var_name, "?", 1) == 0)
		result = ft_itoa(shell->exit_status);
	else if (ft_strlen(var_name) == 1 && ft_strncmp(var_name, "0", 1) == 0)
	{
		if (shell->program_name)
			result = ft_strdup(shell->program_name);
		else
			result = ft_strdup("minishell");
	}
	else
		result = get_env_value(shell->env, var_name);
	free(var_name);
	if (result)
		return (result);
	else
		return (ft_strdup(""));
}
