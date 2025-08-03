/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_shell.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkambarb <gkambarb@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 00:25:13 by gkambarb          #+#    #+#             */
/*   Updated: 2025/08/03 18:40:03 by gkambarb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// OLDPWD initialization:
// In bash, OLDPWD is not inherited from parent environment
// It's a shell-specific variable that starts unset
// Only create OLDPWD when first cd command is used
static void	set_initial_pwd(t_shell *shell)
{
	char	cwd[4096];
	char	*current_dir;

	current_dir = getcwd(cwd, sizeof(cwd));
	if (current_dir)
		set_env_value(shell, "PWD", current_dir);
}

static void	copy_env(t_shell *shell, char **env)
{
	int	i;
	int	env_size;

	env_size = 0;
	while (env[env_size])
		env_size++;
	shell->env = (char **)malloc(sizeof(char *) * (env_size + 1));
	if (!shell->env)
		exit(ERROR);
	i = 0;
	while (i < env_size)
	{
		shell->env[i] = ft_strdup(env[i]);
		if (!shell->env[i])
		{
			free_array(shell->env);
			exit(ERROR);
		}
		i++;
	}
	shell->env[i] = NULL;
}

static void	increment_shlvl(t_shell *shell)
{
	char	*value;
	long	lvl;
	char	*new_value;

	value = get_env_value(shell->env, "SHLVL");
	if (value == NULL)
		lvl = 0;
	else
	{
		lvl = ft_atoi(value);
		free(value);
		if (lvl < 0)
			lvl = 0;
	}
	lvl++;
	new_value = ft_itoa((int)lvl);
	if (!new_value)
		return ;
	set_env_value(shell, "SHLVL", new_value);
	free(new_value);
}

void	init_shell(t_shell *shell, char **env, char *program_name)
{
	char	cwd[4096];

	copy_env(shell, env);
	increment_shlvl(shell);
	shell->commands = NULL;
	shell->exit_status = 0;
	shell->running = 1;
	shell->program_name = ft_strdup(program_name);
	shell->temp_file_counter = 0;
	set_initial_pwd(shell);
	if (getcwd(cwd, sizeof(cwd)))
		shell->pwd = ft_strdup(cwd);
	else
		shell->pwd = ft_strdup("/");
}
