/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+        
	+:+     */
/*   By: wuabdull <wuabdull@student.42.fr>          +#+  +:+      
	+#+        */
/*                                                +#+#+#+#+#+  
	+#+           */
/*   Created: 2025/07/14 13:19:12 by wuabdull          #+#    #+#             */
/*   Updated: 2025/07/14 13:19:12 by wuabdull         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	is_n_flag(const char *s)
{
	int	i;

	if (!s || s[0] != '-' || s[1] != 'n')
		return (0);
	i = 2;
	while (s[i])
	{
		if (s[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

int	ft_echo(t_arg_info *args, int arg_count)
{
	int	i;
	int	n_flag;

	i = 1;
	n_flag = 0;
	while (i < arg_count && is_n_flag(args[i].value))
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
