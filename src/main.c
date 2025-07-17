/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkambarb <gkambarb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 00:10:38 by gkambarb          #+#    #+#             */
/*   Updated: 2025/07/18 00:23:10 by gkambarb         ###   ########.fr       */
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
