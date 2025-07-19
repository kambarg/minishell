/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wuabdull <wuabdull@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 13:19:12 by wuabdull          #+#    #+#             */
/*   Updated: 2025/07/14 13:19:12 by wuabdull         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_echo(t_arg_info *args, int arg_count)
{
	int	i;
	int	n_flag;

	n_flag = 0;
	i = 1;
	while (i < arg_count && ft_strncmp(args[i].value, "-n", 3) == 0)
	{
		n_flag = 1;
		i++;
	}
	while (i < arg_count)
	{
		ft_putstr_fd(args[i].value, STDOUT_FILENO);
		if (i + 1 < arg_count)
			ft_putchar_fd(' ', STDOUT_FILENO);
		i++;
	}
	if (!n_flag)
		ft_putchar_fd('\n', STDOUT_FILENO);
	return (SUCCESS);
}
