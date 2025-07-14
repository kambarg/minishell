#include "../../includes/minishell.h"

static char	*get_var_name(char *str, int *i)
{
	int		start;
	int		len;
	char	*name;

	if (!str || !i)
		return (NULL);
	start = *i;
	(*i)++;
	if (str[*i] == '?')
	{
		(*i)++;
		return (ft_strdup("?"));
	}
	if (str[*i] == '0')
	{
		(*i)++;
		return (ft_strdup("0"));
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

	if (!s)
		return (NULL);
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

	if (!str || !shell || !i)
		return (ft_strdup("$"));
	var_name = get_var_name(str, i);
	if (!var_name)
		return (ft_strdup("$"));
	if (ft_strncmp(var_name, "?", 2) == 0)
	{
		free(var_name);
		result = ft_itoa(shell->exit_status);
		if (result)
			return (result);
		else
			return (ft_strdup(""));
	}
	if (ft_strlen(var_name) == 1 && ft_strncmp(var_name, "0", 1) == 0)
	{
		free(var_name);
		if (shell->program_name)
			result = ft_strdup(shell->program_name);
		else
			result = ft_strdup("minishell");
		if (result)
			return (result);
		else
			return (ft_strdup(""));
	}
	var_value = get_env_value(shell->env, var_name);
	free(var_name);
	if (!var_value)
		return (ft_strdup(""));
	result = ft_strdup(var_value);
	free(var_value);
	if (result)
		return (result);
	else
		return (ft_strdup(""));
}



// Note: single quotes no expansion at all
static char	*expand_quoted_string(char *str, t_shell *shell, int quote_type)
{
	int		i;
	char	*result;
	char	*var;
	char	*new_result;

	if (!str || !shell)
		return (NULL);
	if (quote_type == QUOTE_SINGLE)
		return (ft_strdup(str));
	i = 0;
	result = ft_strdup("");
	if (!result)
		return (NULL);
	while (str[i])
	{
		if (str[i] == '$' && (quote_type == QUOTE_DOUBLE || quote_type == QUOTE_NONE))
		{
			var = expand_var(str, shell, &i);
			if (!var)
				var = ft_strdup("");
			if (!var)
			{
				free(result);
				return (NULL);
			}
			new_result = ft_strjoin(result, var);
			free(var);
			if (!new_result)
			{
				free(result);
				return (NULL);
			}
			free(result);
			result = new_result;
		}
		else
		{
			new_result = ft_strjoin_char(result, str[i]);
			if (!new_result)
			{
				free(result);
				return (NULL);
			}
			free(result);
			result = new_result;
			i++;
		}
	}
	return (result);
}

static char	*safe_expand_string(char *str, t_shell *shell, int quote_type)
{
	char	*result;

	result = expand_quoted_string(str, shell, quote_type);
	if (result)
		return (result);
	
	/* If expansion failed but we need to expand (not single quotes) */
	if (quote_type != QUOTE_SINGLE)
	{
		/* Try to at least handle the most common cases: $? and $0 */
		if (str && ft_strlen(str) == 2 && ft_strncmp(str, "$?", 2) == 0)
		{
			result = ft_itoa(shell->exit_status);
			if (result)
				return (result);
		}
		if (str && ft_strlen(str) == 2 && ft_strncmp(str, "$0", 2) == 0)
		{
			if (shell->program_name)
				result = ft_strdup(shell->program_name);
			else
				result = ft_strdup("minishell");
			if (result)
				return (result);
		}
		/* If that fails too, return empty string as fallback */
		return (ft_strdup(""));
	}
	
	/* For single quotes, return original string */
	return (ft_strdup(str));
}

// Redirections are always expanded
void	expander(t_command *cmd, t_shell *shell)
{
	int			i;
	char		*expanded;
	t_redirect	*redir;

	if (!cmd || !shell)
		return ;
	while (cmd)
	{
		i = 0;
		while (i < cmd->arg_count)
		{
			expanded = safe_expand_string(cmd->args[i].value, shell, cmd->args[i].quote_type);
			if (expanded)
			{
				free(cmd->args[i].value);
				cmd->args[i].value = expanded;
			}
			else
				print_error("expander", "critical memory allocation failed");
			i++;
		}
		redir = cmd->redirects;
		while (redir)
		{
			if (redir->type != REDIR_HEREDOC)
			{
				expanded = safe_expand_string(redir->file, shell, QUOTE_NONE);
				if (expanded)
				{
					free(redir->file);
					redir->file = expanded;
				}
				else
				{
					print_error("expander", "critical memory allocation failed");
				}
			}
			redir = redir->next;
		}
		cmd = cmd->next;
	}
} 
