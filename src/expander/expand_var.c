/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_var.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wuabdull <wuabdull@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 14:10:06 by gkambarb          #+#    #+#             */
/*   Updated: 2025/07/21 20:34:10 by wuabdull         ###   ########.fr       */
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

char	*expand_special_vars(char *var_name, t_shell *shell)
{
	if (ft_strncmp(var_name, "?", 2) == 0)
		return (ft_itoa(shell->exit_status));
	else if (ft_strncmp(var_name, "0", 2) == 0)
	{
		if (shell->program_name)
			return (ft_strdup(shell->program_name));
		else
			return (ft_strdup("minishell"));
	}
	else
		return (get_env_value(shell->env, var_name));
}

char	*expand_nonalpha(char *str, t_shell *shell, int *i)
{
	char	temp[2];
	char	*result;

	temp[0] = str[*i + 1];
	temp[1] = '\0';
	(*i)++;
	result = get_env_value(shell->env, temp);
	if (!result)
		result = ft_strdup("");
	(*i)++;
	return (result);
}

char	*expand_var(char *str, t_shell *shell, int *i)
{
	char	*var_name;
	char	*result;

	if (!str || !shell || !i)
		return (ft_strdup("$"));
	if (!ft_isalpha(str[*i + 1]) && str[*i + 1] != '_' && str[*i + 1] != '?'
		&& str[*i + 1] != '0')
		return (expand_nonalpha(str, shell, i));
	var_name = get_var_name(str, i);
	if (!var_name)
		return (ft_strdup("$"));
	result = expand_special_vars(var_name, shell);
	free(var_name);
	if (result)
		return (result);
	else
		return (ft_strdup(""));
}
