#include "../../includes/minishell.h"

int	ft_pwd(void)
{
	char	cwd[4096];

	if (!getcwd(cwd, sizeof(cwd)))
	{
		print_error("pwd", strerror(errno));
		return (ERROR);
	}
	ft_putendl_fd(cwd, STDOUT_FILENO);
	return (SUCCESS);
} 