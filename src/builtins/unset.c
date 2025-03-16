#include "../../includes/minishell.h"

static void	remove_env_var(t_shell *shell, const char *name)
{
	int		i;
	int		j;
	int		len;
	char	**new_env;
	int		env_size;

	len = ft_strlen(name);
	env_size = 0;
	while (shell->env[env_size])
		env_size++;
	new_env = (char **)malloc(sizeof(char *) * env_size);
	if (!new_env)
		return ;
	i = 0;
	j = 0;
	while (shell->env[i])
	{
		if (ft_strncmp(shell->env[i], name, len) != 0 ||
			shell->env[i][len] != '=')
		{
			new_env[j] = ft_strdup(shell->env[i]);
			j++;
		}
		i++;
	}
	new_env[j] = NULL;
	free_array(shell->env);
	shell->env = new_env;
}

int	ft_unset(char **args, t_shell *shell)
{
	int	i;

	i = 1;
	while (args[i])
	{
		remove_env_var(shell, args[i]);
		i++;
	}
	return (SUCCESS);
} 