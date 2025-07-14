#include "../../includes/minishell.h"

static void	print_sorted_env(char **env)
{
	int		i;
	int		j;
	int		size;
	char	*tmp;

	size = 0;
	while (env[size])
		size++;
	i = 0;
	while (i < size - 1)
	{
		j = 0;
		while (j < size - i - 1)
		{
			if (ft_strncmp(env[j], env[j + 1], ft_strlen(env[j]) + 1) > 0)
			{
				tmp = env[j];
				env[j] = env[j + 1];
				env[j + 1] = tmp;
			}
			j++;
		}
		i++;
	}
	i = 0;
	while (env[i])
	{
		ft_putstr_fd("declare -x ", STDOUT_FILENO);
		ft_putendl_fd(env[i], STDOUT_FILENO);
		i++;
	}
}

static int	is_valid_identifier(char *str)
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

static void	handle_export_arg(char *arg, t_shell *shell)
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

int	ft_export(t_arg_info *args, int arg_count, t_shell *shell)
{
	int		i;
	char	**env_copy;
	int		env_size;
	int		has_valid_args;

	/* Check if we have any non-empty arguments */
	has_valid_args = 0;
	i = 1;
	while (i < arg_count)
	{
		if (args[i].value && *args[i].value)
		{
			has_valid_args = 1;
			break;
		}
		i++;
	}

	/* If no arguments or all arguments are empty strings, list variables */
	if (arg_count < 2 || !has_valid_args)
	{
		env_size = 0;
		while (shell->env[env_size])
			env_size++;
		env_copy = (char **)malloc(sizeof(char *) * (env_size + 1));
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

	/* Process non-empty arguments */
	i = 1;
	while (i < arg_count)
	{
		if (args[i].value && *args[i].value)
		{
			handle_export_arg(args[i].value, shell);
		}
		/* Skip empty arguments silently */
		i++;
	}
	return (SUCCESS);
} 