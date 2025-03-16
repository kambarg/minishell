#include "../../includes/minishell.h"

char	*get_env_value(char **env, const char *name)
{
	int		i;
	int		len;
	char	*value;

	i = 0;
	len = ft_strlen(name);
	while (env[i])
	{
		if (ft_strncmp(env[i], name, len) == 0 && env[i][len] == '=')
		{
			value = ft_strdup(env[i] + len + 1);
			return (value);
		}
		i++;
	}
	return (NULL);
}

static int	get_env_index(char **env, const char *name)
{
	int	i;
	int	len;

	i = 0;
	len = ft_strlen(name);
	while (env[i])
	{
		if (ft_strncmp(env[i], name, len) == 0 && env[i][len] == '=')
			return (i);
		i++;
	}
	return (-1);
}

void	set_env_value(t_shell *shell, const char *name, const char *value)
{
	int		i;
	int		env_size;
	char	*new_var;
	char	**new_env;

	if (!name || !value)
		return;
		
	i = get_env_index(shell->env, name);
	if (i >= 0)
	{
		free(shell->env[i]);
		new_var = ft_strjoin(name, "=");
		shell->env[i] = ft_strjoin(new_var, value);
		free(new_var);
	}
	else
	{
		env_size = 0;
		while (shell->env[env_size])
			env_size++;
		new_env = (char **)malloc(sizeof(char *) * (env_size + 2));
		if (!new_env)
			return;
		i = -1;
		while (++i < env_size)
			new_env[i] = shell->env[i];
		new_var = ft_strjoin(name, "=");
		new_env[i] = ft_strjoin(new_var, value);
		new_env[i + 1] = NULL;
		free(new_var);
		free(shell->env);
		shell->env = new_env;
	}
} 