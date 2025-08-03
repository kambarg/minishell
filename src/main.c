/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkambarb <gkambarb@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 00:10:38 by gkambarb          #+#    #+#             */
/*   Updated: 2025/08/03 18:34:07 by gkambarb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	cleanup_shell(t_shell *shell)
{
	if (shell->env)
		free_array(shell->env);
	if (shell->commands)
		free_commands(shell->commands);
	if (shell->program_name)
		free(shell->program_name);
	if (shell->pwd)
		free(shell->pwd);
	clear_history();
}

int	main(int argc, char **argv, char **env)
{
	t_shell	shell;

	(void)argc;
	setup_signals_interactive();
	init_shell(&shell, env, argv[0]);
	run_shell(&shell);
	cleanup_shell(&shell);
	return (shell.exit_status);
}
