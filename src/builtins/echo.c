#include "../../includes/minishell.h"

int	ft_echo(t_arg_info *args, int arg_count)
{
	int	i;
	int	n_flag;

	n_flag = 0;
	i = 1;
	if (arg_count > 1 && ft_strncmp(args[1].value, "-n", 3) == 0)
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