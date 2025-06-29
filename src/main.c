#include "../includes/minishell.h"
#include <readline/readline.h>
#include <readline/history.h>

void	init_shell(t_shell *shell, char **env)
{
	int	i;
	int	env_size;
	char	cwd[4096];
	char	*current_dir;

	env_size = 0;
	while (env[env_size])
		env_size++;
	shell->env = (char **)malloc(sizeof(char *) * (env_size + 1));
	if (!shell->env)
		exit(ERROR);
	i = 0;
	while (i < env_size)
	{
		shell->env[i] = ft_strdup(env[i]);
		if (!shell->env[i])
		{
			free_array(shell->env);
			exit(ERROR);
		}
		i++;
	}
	shell->env[i] = NULL;
	shell->commands = NULL;
	shell->exit_status = 0;
	shell->running = 1;
	
	/* Initialize PWD and OLDPWD with the current directory */
	current_dir = getcwd(cwd, sizeof(cwd));
	if (current_dir)
	{
		set_env_value(shell, "PWD", current_dir);
		set_env_value(shell, "OLDPWD", current_dir);
	}
}

void	run_shell(t_shell *shell)
{
	char	*input;
	t_token	*tokens;

	while (shell->running)
	{
		setup_signals_interactive();
		input = readline("minishell$ ");
		if (!input || g_exec_status == 3)
		{
			if (!input)
				printf("exit\n");
			if (g_exec_status == 3)
				shell->exit_status = 130; /* 130 is the standard exit code for SIGINT */
			if (input)
				free(input);
			break;
		}
		if (*input && !is_whitespace_only(input))
		{
			add_history(input);
			tokens = lexer(input);
			if (tokens)
			{
				/* Validate token stream for syntax errors */
				if (validate_tokens(tokens))
				{
					shell->commands = parser(tokens);
					if (shell->commands)
					{
						expand_variables(shell->commands, shell);
						setup_signals_executing();
						shell->exit_status = execute_commands(shell);
						setup_signals_interactive();
						free_commands(shell->commands);
						shell->commands = NULL;
					}
				}
				else
				{
					shell->exit_status = 258; /* Syntax error (exit status = err code 2 + 256) */
				}
				free_tokens(tokens);
			}
		}
		free(input);
	}
}

void	cleanup_shell(t_shell *shell)
{
	if (shell->env)
		free_array(shell->env);
	if (shell->commands)
		free_commands(shell->commands);
	rl_clear_history();
}

int	main(int argc, char **argv, char **env)
{
	t_shell	shell;

	(void)argc;
	(void)argv;
	setup_signals_interactive();
	init_shell(&shell, env);
	run_shell(&shell);
	cleanup_shell(&shell);
	return (shell.exit_status);
} 