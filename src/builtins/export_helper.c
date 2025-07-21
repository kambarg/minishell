/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_helper.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkambarb <gkambarb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 13:27:08 by wuabdull          #+#    #+#             */
/*   Updated: 2025/07/18 11:05:41 by gkambarb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	is_valid_identifier(char *str)
{
	int	i;

	if (!str || !*str || (!ft_isalpha(*str) && *str != '_'))
		return (0);
	i = 1;
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

static char	*strip_quotes(char *str)
{
	int		len;
	char	*result;

	if (!str)
		return (NULL);
	len = ft_strlen(str);
	if (len >= 2 && ((str[0] == '"' && str[len - 1] == '"') || (str[0] == '\''
				&& str[len - 1] == '\'')))
	{
		result = ft_substr(str, 1, len - 2);
	}
	else
	{
		result = ft_strdup(str);
	}
	return (result);
}

void	handle_export_arg(char *arg, t_shell *shell)
{
	char	*name;
	char	*value;
	char	*raw_value;
	char	*equals;

	if (!is_valid_identifier(arg))
	{
		print_error("export", "not a valid identifier");
		return ;
	}
	equals = ft_strchr(arg, '=');
	if (!equals)
		return ;
	name = ft_substr(arg, 0, equals - arg);
	raw_value = ft_strdup(equals + 1);
	value = strip_quotes(raw_value);
	free(raw_value);
	set_env_value(shell, name, value);
	free(name);
	free(value);
}
