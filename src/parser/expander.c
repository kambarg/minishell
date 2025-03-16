#include "../../includes/minishell.h"

static char	*get_var_name(char *str, int *i)
{
	int		start;
	int		len;
	char	*name;

	start = *i;
	(*i)++;
	if (str[*i] == '?')
	{
		(*i)++;
		return (ft_strdup("?"));
	}
	while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_'))
		(*i)++;
	len = *i - start - 1;
	if (len == 0)
		return (NULL);
	name = (char *)malloc(sizeof(char) * (len + 1));
	if (!name)
		return (NULL);
	ft_strlcpy(name, str + start + 1, len + 1);
	return (name);
}

static char	*ft_strjoin_char(char *s, char c)
{
	char	*result;
	int		len;
	int		i;

	len = ft_strlen(s);
	result = (char *)malloc(sizeof(char) * (len + 2));
	if (!result)
		return (NULL);
	i = 0;
	while (s[i])
	{
		result[i] = s[i];
		i++;
	}
	result[i] = c;
	result[i + 1] = '\0';
	return (result);
}

static char	*expand_var(char *str, t_shell *shell, int *i)
{
	char	*var_name;
	char	*var_value;
	char	*result;

	var_name = get_var_name(str, i);
	if (!var_name)
		return (ft_strdup("$"));
	if (ft_strncmp(var_name, "?", 2) == 0)
	{
		free(var_name);
		return (ft_itoa(shell->exit_status));
	}
	var_value = get_env_value(shell->env, var_name);
	free(var_name);
	if (!var_value)
		return (ft_strdup(""));
	result = ft_strdup(var_value);
	free(var_value);
	return (result);
}

static char	*expand_string(char *str, t_shell *shell)
{
	int		i;
	char	*result;
	char	*temp;
	char	*var;

	i = 0;
	result = ft_strdup("");
	while (str[i])
	{
		if (str[i] == '$')
		{
			var = expand_var(str, shell, &i);
			temp = result;
			result = ft_strjoin(result, var);
			free(temp);
			free(var);
		}
		else
		{
			temp = result;
			result = ft_strjoin_char(result, str[i]);
			free(temp);
			i++;
		}
	}
	return (result);
}

void	expand_variables(t_command *cmd, t_shell *shell)
{
	int			i;
	char		*expanded;
	t_redirect	*redir;

	while (cmd)
	{
		i = 0;
		while (cmd->args && cmd->args[i])
		{
			expanded = expand_string(cmd->args[i], shell);
			free(cmd->args[i]);
			cmd->args[i] = expanded;
			i++;
		}
		redir = cmd->redirects;
		while (redir)
		{
			if (redir->type != REDIR_HEREDOC)
			{
				expanded = expand_string(redir->file, shell);
				free(redir->file);
				redir->file = expanded;
			}
			redir = redir->next;
		}
		cmd = cmd->next;
	}
} 