/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_shell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkambarb <gkambarb@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 03:19:17 by gkambarb          #+#    #+#             */
/*   Updated: 2025/08/03 18:00:26 by gkambarb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	handle_input_exit(t_shell *shell, char *input)
{
	int	tty_fd;

	if (g_exec_status == 3)
	{
		tty_fd = open("/dev/tty", O_RDONLY);
		if (tty_fd == -1)
		{
			perror("open /dev/tty");
			exit(EXIT_FAILURE);
		}
		if (tty_fd != STDIN_FILENO)
		{
			dup2(tty_fd, STDIN_FILENO);
			close(tty_fd);
		}
		shell->exit_status = 130;
		g_exec_status = 0;
		return (0);
	}
	if (!input)
	{
		printf("exit\n");
		return (1);
	}
	return (0);
}

static void	process_valid_input(t_shell *shell, t_token *tokens)
{
	if (validate_tokens(tokens))
	{
		shell->commands = parser(tokens);
		if (shell->commands)
		{
			expander(shell->commands, shell);
			setup_signals_executing();
			shell->exit_status = execute_commands(shell);
			setup_signals_interactive();
			free_commands(shell->commands);
			shell->commands = NULL;
		}
	}
	else
	{
		shell->exit_status = 258;
	}
}

static void	process_input_line(t_shell *shell, char *input)
{
	t_token	*tokens;

	printf("DEBUG: input = %s\n", input);
	if (input == NULL)
		input = "exit\n";
	if (*input && !is_whitespace_only(input))
	{
		add_history(input);
		tokens = lexer(input);
		if (tokens)
		{
			process_valid_input(shell, tokens);
			free_tokens(tokens);
		}
	}
}

void	run_shell(t_shell *shell)
{
	char	*input;

	while (shell->running)
	{
		setup_signals_interactive();
		input = readline("minishell$ ");
		if (handle_input_exit(shell, input))
			break ;
		process_input_line(shell, input);
		free(input);
	}
}
