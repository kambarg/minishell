#include "../../includes/minishell.h"

static int	is_numeric(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

int	ft_exit(char **args, t_shell *shell)
{
	int	exit_code;

	ft_putendl_fd("exit", STDERR_FILENO);
	if (!args[1])
	{
		shell->running = 0;
		return (shell->exit_status);
	}
	if (!is_numeric(args[1]))
	{
		print_error("exit", "numeric argument required");
		shell->running = 0;
		return (255);
	}
	if (args[2])
	{
		print_error("exit", "too many arguments");
		return (ERROR);
	}
	exit_code = ft_atoi(args[1]) & 255;
	shell->running = 0;
	return (exit_code);
} 