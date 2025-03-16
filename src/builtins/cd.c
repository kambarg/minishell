#include "../../includes/minishell.h"

int	ft_cd(char **args, t_shell *shell)
{
	char	*path;
	char	*old_pwd;
	char	*new_pwd;
	char	cwd[4096];

	/* Get current directory before changing */
	old_pwd = getcwd(cwd, sizeof(cwd));
	if (!old_pwd)
	{
		print_error("cd", strerror(errno));
		return (ERROR);
	}
	
	/* Handle cd with no arguments (go to HOME) */
	if (!args[1])
		path = get_env_value(shell->env, "HOME");
	/* Handle cd - (go to previous directory) */
	else if (ft_strncmp(args[1], "-", 2) == 0)
	{
		path = get_env_value(shell->env, "OLDPWD");
		if (path)
			ft_putendl_fd(path, STDOUT_FILENO); /* Print the directory we're changing to */
	}
	/* Handle cd with a specific path */
	else
		path = ft_strdup(args[1]);
	
	/* Check if we have a valid path */
	if (!path)
	{
		if (args[1] && ft_strncmp(args[1], "-", 2) == 0)
			print_error("cd", "OLDPWD not set");
		else
			print_error("cd", "HOME not set");
		return (ERROR);
	}
	
	/* Change directory */
	if (chdir(path) == -1)
	{
		print_error("cd", strerror(errno));
		free(path);
		return (ERROR);
	}
	free(path);
	
	/* Update PWD and OLDPWD environment variables */
	new_pwd = getcwd(cwd, sizeof(cwd));
	if (!new_pwd)
	{
		print_error("cd", strerror(errno));
		return (ERROR);
	}
	
	/* Set environment variables */
	set_env_value(shell, "OLDPWD", old_pwd);
	set_env_value(shell, "PWD", new_pwd);
	
	return (SUCCESS);
} 