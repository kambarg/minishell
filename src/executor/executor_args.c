/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_args.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wuabdull <wuabdull@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 16:48:05 by wuabdull          #+#    #+#             */
/*   Updated: 2025/07/15 16:48:15 by wuabdull         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	**create_argv(t_command *cmd)
{
	char	**argv;
	int		i;

	if (cmd->arg_count == 0)
		return (NULL);
	argv = (char **)malloc(sizeof(char *) * (cmd->arg_count + 1));
	if (!argv)
		return (NULL);
	i = 0;
	while (i < cmd->arg_count)
	{
		argv[i] = cmd->args[i].value;
		i++;
	}
	argv[i] = NULL;
	return (argv);
}
