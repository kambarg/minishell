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
		return ((*i)++, ft_strdup("?"));
	if (str[*i] == '0')
		return ((*i)++, ft_strdup("0"));
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
	char	*result;

	if (!str || !shell || !i)
		return (ft_strdup("$"));
	var_name = get_var_name(str, i);
	if (!var_name)
		return (ft_strdup("$"));
	if (ft_strlen(var_name) == 1 && ft_strncmp(var_name, "?", 1) == 0)
		result = ft_itoa(shell->exit_status);
	else if (ft_strlen(var_name) == 1 && ft_strncmp(var_name, "0", 1) == 0)
	{
		if (shell->program_name)
			result = ft_strdup(shell->program_name);
		else
			result = ft_strdup("minishell");
	}
	else
		result = get_env_value(shell->env, var_name);
	free(var_name);
	if (result)
		return (result);
	else
		return (ft_strdup(""));
}

static char	*handle_dollar_char(char *str, t_shell *shell, int *i, char *result)
{
	char	*var;
	char	*new_result;

	var = expand_var(str, shell, i);
	if (!var)
		return (free(result), NULL);
	new_result = ft_strjoin(result, var);
	free(var);
	if (!new_result)
		return (free(result), NULL);
	free(result);
	return (new_result);
}

static char	*handle_regular_char(char *result, char c)
{
	char	*new_result;

	new_result = ft_strjoin_char(result, c);
	if (!new_result)
		return (free(result), NULL);
	free(result);
	return (new_result);
}

// Note: single quotes no expansion at all
static char	*expand_quoted_string(char *str, t_shell *shell, int quote_type)
{
	int		i;
	char	*result;

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
			result = handle_dollar_char(str, shell, &i, result);
		else
			result = handle_regular_char(result, str[i++]);
		if (!result)
			return (NULL);
	}
	return (result);
}

static void	expand_args(t_command *cmd, t_shell *shell)
{
	int		i;
	char	*expanded;

	i = 0;
	while (i < cmd->arg_count)
	{
		expanded = expand_quoted_string(cmd->args[i].value, shell, cmd->args[i].quote_type);
		if (expanded)
		{
			free(cmd->args[i].value);
			cmd->args[i].value = expanded;
		}
		else
			print_error("expander", "critical memory allocation failed");
		i++;
	}
}

// Note: redirections are always expanded
static void	expand_redirects(t_command *cmd, t_shell *shell)
{
	t_redirect	*redir;
	char		*expanded;

	redir = cmd->redirects;
	while (redir)
	{
		if (redir->type != REDIR_HEREDOC)
		{
			expanded = expand_quoted_string(redir->file, shell, QUOTE_NONE);
			if (expanded)
			{
				free(redir->file);
				redir->file = expanded;
			}
			else
				print_error("expander", "critical memory allocation failed");
		}
		redir = redir->next;
	}
}

void	expander(t_command *cmd, t_shell *shell)
{
	if (!cmd || !shell)
		return ;
	while (cmd)
	{
		expand_args(cmd, shell);
		expand_redirects(cmd, shell);
		cmd = cmd->next;
	}
}
