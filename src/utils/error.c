#include "../../includes/minishell.h"

void	print_error(const char *cmd, const char *msg)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	if (cmd)
	{
		ft_putstr_fd((char *)cmd, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
	}
	ft_putendl_fd((char *)msg, STDERR_FILENO);
}

int	is_whitespace_only(const char *str)
{
	if (!str)
		return (1);
	while (*str)
	{
		if (*str != ' ' && *str != '\t' && *str != '\n' && *str != '\r')
			return (0);
		str++;
	}
	return (1);
} 