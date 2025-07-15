/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_helper.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wuabdull <wuabdull@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 13:27:08 by wuabdull          #+#    #+#             */
/*   Updated: 2025/07/14 14:27:48 by wuabdull         ###   ########.fr       */
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

void	handle_export_arg(char *arg, t_shell *shell)
{
	char	*name;
	char	*value;
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
	value = ft_strdup(equals + 1);
	set_env_value(shell, name, value);
	free(name);
	free(value);
}
