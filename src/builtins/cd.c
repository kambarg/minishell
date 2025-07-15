#include "../../includes/minishell.h"

int	ft_cd(t_arg_info *args, int arg_count, t_shell *shell)
{
	char	*path;
	char	*old_pwd;
	char	*new_pwd;
	char	cwd[4096];
	char	*old_pwd_copy;

	/* Get current directory before changing */
	old_pwd = getcwd(cwd, sizeof(cwd));
	if (!old_pwd)
	{
		print_error("cd", strerror(errno));
		return (ERROR);
	}
	
	/* Make a copy of the old directory to avoid buffer overwrite */
	old_pwd_copy = ft_strdup(old_pwd);
	if (!old_pwd_copy)
	{
		print_error("cd", strerror(errno));
		return (ERROR);
	}
	
	/* Handle cd with no arguments (go to HOME) */
	if (arg_count < 2)
		path = get_env_value(shell->env, "HOME");
	/* Handle cd - (go to previous directory) */
	else if (ft_strncmp(args[1].value, "-", 2) == 0)
	{
		path = get_env_value(shell->env, "OLDPWD");
		if (path)
			ft_putendl_fd(path, STDOUT_FILENO); /* Print the directory we're changing to */
	}
	/* Handle cd with a specific path */
	else
		path = ft_strdup(args[1].value);
	
	/* Check if we have a valid path */
	if (!path)
	{
		if (arg_count > 1 && ft_strncmp(args[1].value, "-", 2) == 0)
			print_error("cd", "OLDPWD not set");
		else
			print_error("cd", "HOME not set");
		free(old_pwd_copy);
		return (ERROR);
	}
	
	/* Change directory */
	if (chdir(path) == -1)
	{
		print_error("cd", strerror(errno));
		free(path);
		free(old_pwd_copy);
		return (ERROR);
	}
	free(path);
	
	/* Update PWD and OLDPWD environment variables */
	new_pwd = getcwd(cwd, sizeof(cwd));
	if (!new_pwd)
	{
		print_error("cd", strerror(errno));
		free(old_pwd_copy);
		return (ERROR);
	}
	
	/* Set environment variables */
	set_env_value(shell, "OLDPWD", old_pwd_copy);
	set_env_value(shell, "PWD", new_pwd);
	
	/* Clean up */
	free(old_pwd_copy);
	
	return (SUCCESS);
} 