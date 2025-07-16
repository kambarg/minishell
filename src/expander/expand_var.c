char	*expand_var(char *str, t_shell *shell, int *i)
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
