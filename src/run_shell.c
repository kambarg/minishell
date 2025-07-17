/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_shell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkambarb <gkambarb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 00:25:25 by gkambarb          #+#    #+#             */
/*   Updated: 2025/07/18 00:25:26 by gkambarb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// Print "syntax error near unexpected token" if
// pipe followed by nothing or another pipe
// redirection without target
int	validate_tokens(t_token *tokens)
{
	t_token	*current;
	t_token	*next;

	if (!tokens)
		return (1);
	current = tokens;
	while (current)
	{
		next = current->next;
		if (current->type == T_PIPE && (!next || next->type == T_PIPE))
		{
			print_error(NULL, "syntax error near unexpected token `|'");
			return (0);
		}
		if ((current->type >= T_REDIR_IN && current->type <= T_APPEND)
			&& (!next || next->type != T_WORD))
		{
			print_error(NULL, "syntax error near unexpected token `newline'");
			return (0);
		}
		current = current->next;
	}
	return (1);
}

static int	handle_input_exit(t_shell *shell, char *input)
{
	if (!input || g_exec_status == 3)
	{
		if (!input)
			printf("exit\n");
		if (g_exec_status == 3)
			shell->exit_status = 130;
		if (input)
			free(input);
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
