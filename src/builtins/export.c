/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wuabdull <wuabdull@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 13:20:14 by wuabdull          #+#    #+#             */
/*   Updated: 2025/07/14 13:20:14 by wuabdull         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	has_valid_export_arg(t_arg_info *args, int arg_count)
{
	int	i;

	i = 1;
	while (i < arg_count)
	{
		if (args[i].value && *args[i].value)
			return (1);
		i++;
	}
	return (0);
}

static int	export_print_env(t_shell *shell)
{
	char	**env_copy;
	int		env_size;
	int		i;

	env_size = 0;
	while (shell->env[env_size])
		env_size++;
	env_copy = (char **)malloc(sizeof(char *) * (env_size + 1));
	if (!env_copy)
		return (ERROR);
	i = 0;
	while (i < env_size)
	{
		env_copy[i] = ft_strdup(shell->env[i]);
		i++;
	}
	env_copy[i] = NULL;
	print_sorted_env(env_copy);
	free_array(env_copy);
	return (SUCCESS);
}

static void	export_update_env(t_arg_info *args, int arg_count, t_shell *shell)
{
	int	i;

	i = 1;
	while (i < arg_count)
	{
		if (args[i].value && *args[i].value)
			handle_export_arg(args[i].value, shell);
		i++;
	}
}

int	ft_export(t_arg_info *args, int arg_count, t_shell *shell)
{
	if (arg_count < 2 || !has_valid_export_arg(args, arg_count))
		return (export_print_env(shell));
	export_update_env(args, arg_count, shell);
	return (SUCCESS);
}
