/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkambarb <gkambarb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 09:59:14 by gkambarb          #+#    #+#             */
/*   Updated: 2025/07/10 10:01:22 by gkambarb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static t_arg_info	*create_argument(char *value, int quote_type)
{
	t_arg_info	*arg;

	arg = (t_arg_info *)malloc(sizeof(t_arg_info));
	if (!arg)
		return (NULL);
	arg->value = ft_strdup(value);
	if (!arg->value)
	{
		free(arg);
		return (NULL);
	}
	arg->quote_type = quote_type;
	return (arg);
}

int	add_argument(t_command *cmd, char *value, int quote_type)
{
	t_arg_info	*new_args;
	t_arg_info	*new_arg;
	int			i;

	new_arg = create_argument(value, quote_type);
	if (!new_arg)
		return (0);
	new_args = (t_arg_info *)malloc(sizeof(t_arg_info) * (cmd->arg_count + 1));
	if (!new_args)
	{
		free(new_arg->value);
		free(new_arg);
		return (0);
	}
	i = 0;
	while (i < cmd->arg_count)
	{
		new_args[i] = cmd->args[i];
		i++;
	}
	new_args[i] = *new_arg;
	free(new_arg);
	free(cmd->args);
	cmd->args = new_args;
	cmd->arg_count++;
	return (1);
}
