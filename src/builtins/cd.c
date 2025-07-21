/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wuabdull <wuabdull@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 15:41:41 by wuabdull          #+#    #+#             */
/*   Updated: 2025/07/15 15:41:41 by wuabdull         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	cd_get_oldpwd(char *cwd, char **old_pwd_copy)
{
	char	*old_pwd;

	old_pwd = getcwd(cwd, sizeof(char) * 4096);
	if (!old_pwd)
	{
		print_error("cd", strerror(errno));
		return (ERROR);
	}
	*old_pwd_copy = ft_strdup(old_pwd);
	if (!*old_pwd_copy)
	{
		print_error("cd", strerror(errno));
		return (ERROR);
	}
	return (SUCCESS);
}

static char	*cd_choose_path(t_arg_info *args, int arg_count, t_shell *shell)
{
	char	*path;

	if (arg_count < 2)
		path = get_env_value(shell->env, "HOME");
	else if (ft_strncmp(args[1].value, "-", 2) == 0)
	{
		path = get_env_value(shell->env, "OLDPWD");
		if (path)
			ft_putendl_fd(path, STDOUT_FILENO);
	}
	else if (ft_strncmp(args[1].value, "~", 2) == 0)
		path = get_env_value(shell->env, "HOME");
	else
		path = ft_strdup(args[1].value);
	return (path);
}



static int	cd_handle_no_path(t_arg_info *args, int arg_count,
		char *old_pwd_copy)
{
	if (arg_count > 1 && ft_strncmp(args[1].value, "-", 2) == 0)
		print_error("cd", "OLDPWD not set");
	else
		print_error("cd", "HOME not set");
	free(old_pwd_copy);
	return (ERROR);
}

static int	cd_update_env(t_shell *shell, char *old_pwd_copy, char *cwd)
{
	char	*new_pwd;

	new_pwd = getcwd(cwd, sizeof(char) * 4096);
	if (!new_pwd)
	{
		print_error("cd", strerror(errno));
		free(old_pwd_copy);
		return (ERROR);
	}
	set_env_value(shell, "OLDPWD", old_pwd_copy);
	set_env_value(shell, "PWD", new_pwd);
	free(old_pwd_copy);
	return (SUCCESS);
}

int	ft_cd(t_arg_info *args, int arg_count, t_shell *shell)
{
	char	cwd[4096];
	char	*old_pwd_copy;
	char	*path;

	if (cd_get_oldpwd(cwd, &old_pwd_copy) == ERROR)
		return (ERROR);
	path = cd_choose_path(args, arg_count, shell);
	if (!path)
		return (cd_handle_no_path(args, arg_count, old_pwd_copy));
	if (chdir(path) == -1)
	{
		print_error("cd", strerror(errno));
		free(path);
		free(old_pwd_copy);
		return (ERROR);
	}
	free(path);
	return (cd_update_env(shell, old_pwd_copy, cwd));
}
