/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkambarb <gkambarb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 10:50:26 by gkambarb          #+#    #+#             */
/*   Updated: 2025/07/23 18:25:30 by gkambarb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <dirent.h>
# include <errno.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h> // <editline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <unistd.h>

# define SUCCESS 0
# define ERROR 1

// Token types
# define T_WORD 1
# define T_PIPE 2
# define T_REDIR_IN 3
# define T_REDIR_OUT 4
# define T_HEREDOC 5
# define T_APPEND 6

// Quote types
# define QUOTE_NONE 0
# define QUOTE_SINGLE 1
# define QUOTE_DOUBLE 2

// Redirection types
# define REDIR_IN 1
# define REDIR_OUT 2
# define REDIR_HEREDOC 3
# define REDIR_APPEND 4

typedef struct s_token
{
	char				*value;
	int					type;
	int					quote_type;
	struct s_token		*next;
}						t_token;

// fd: file descriptor for heredoc temp file
typedef struct s_redirect
{
	int					type;
	char				*file;
	int					fd;
	int					quote_type;
	struct s_redirect	*next;
}						t_redirect;

typedef struct s_arg_info
{
	char				*value;
	int					quote_type;
}						t_arg_info;

typedef struct s_command
{
	t_arg_info			*args;
	int					arg_count;
	t_redirect			*redirects;
	struct s_command	*next;
	int					pipe_fd[2];
}						t_command;

// program_name = argv[0]
// temp_file_counter - counter for unique temp file names
typedef struct s_shell
{
	char				**env;
	t_command			*commands;
	int					exit_status;
	int					running;
	char				*program_name;
	int					temp_file_counter;
}						t_shell;

// Global variables
// 0: interactive, 1: executing command, 2: heredoc, 3: if SIGINT was received
extern int				g_exec_status;

// Main functions
int						validate_tokens(t_token *tokens);
void					init_shell(t_shell *shell, char **env,
							char *program_name);
void					run_shell(t_shell *shell);

// Lexer functions
t_token					*lexer(char *input);
int						handle_operator(char *input, int *i, t_token **tokens);
int						handle_quoted_string(char *input, int *i,
							t_token **tokens);
int						handle_word(char *input, int *i, t_token **tokens);
t_token					*create_token(char *value, int type, int quote_type);
void					add_token(t_token **head, t_token *new_token);
int						is_whitespace(char c);
int						is_operator_char(char c);
int						is_quotes(char c);

// Parser functions
t_command				*parser(t_token *tokens);
int						add_argument(t_command *cmd, char *value,
							int quote_type);
int						handle_redirect(t_token **token, t_command *cmd);

// Expander functions
void					expander(t_command *cmd, t_shell *shell);
char					*expand_var(char *str, t_shell *shell, int *i);
char					*expand_quoted_string(char *str, t_shell *shell,
							int quote_type);

// Executor functions
int						execute_commands(t_shell *shell);
int						handle_redirections(t_redirect *redirects);
int						preprocess_heredocs(t_command *commands,
							t_shell *shell);
void					setup_pipes(t_command *cmd);
char					*find_command_path(char *cmd, char **env);
int						execute_builtin(t_command *cmd, t_shell *shell);
int						is_builtin(char *cmd);
char					**create_argv(t_command *cmd);

// executor_args.c
char					**create_argv(t_command *cmd);

// executor_builtin.c
int						execute_builtin(t_command *cmd, t_shell *shell);
int						handle_builtin_cmd(t_command *cmd, t_shell *shell,
							int stdin_backup, int stdout_backup);

// executor_path.c
char					*find_command_path(char *cmd, char **env);

// executor_pipes.c
void					init_pipe_fds_loop(t_command *cmd);
void					setup_pipes_loop(t_command *cmd);
void					close_all_pipes(t_command *cmd);

// executor_external.c
void					run_external_child(t_command *cmd, t_shell *shell);
int						handle_external_cmd(t_command *cmd, t_shell *shell,
							int *last_pid);

// executor_loop.c
int						run_command_loop(t_shell *shell, int stdin_backup,
							int stdout_backup, pid_t *last_pid);
int						wait_and_return_status(pid_t last_pid, int stdin_backup,
							int stdout_backup);

// Builtin functions
int						ft_echo(t_arg_info *args, int arg_count);
int						ft_cd(t_arg_info *args, int arg_count, t_shell *shell);
int						ft_pwd(void);
int						ft_export(t_arg_info *args, int arg_count,
							t_shell *shell);
int						ft_unset(t_arg_info *args, int arg_count,
							t_shell *shell);
int						ft_env(t_shell *shell);
int						ft_exit(t_arg_info *args, int arg_count,
							t_shell *shell);
void					handle_export_arg(char *arg, t_shell *shell);
int						is_valid_identifier(char *str);
int						get_env_size(char **env);
void					bubble_sort_env(char **env, int size);
void					print_env_lines(char **env);
void					print_sorted_env(char **env);

// Utils functions
void					handle_signals(int signum);
void					setup_signals_interactive(void);
void					setup_signals_executing(void);
void					setup_signals_heredoc(void);
void					reset_signals_default(void);
char					*get_env_value(char **env, const char *name);
void					set_env_value(t_shell *shell, const char *name,
							const char *value);
void					set_env_no_value(t_shell *shell, const char *name);
int						append_env_entry(t_shell *shell, const char *name,
							const char *value);
void					print_error(const char *cmd, const char *msg);
int						is_whitespace_only(const char *str);

// Memory management
void					free_tokens(t_token *tokens);
void					free_commands(t_command *commands);
void					free_array(char **array);

// Temp file management
char					*create_temp_path(t_shell *shell);
int						create_temp_file(t_redirect *redir, int *temp_fd,
							t_shell *shell);

#endif