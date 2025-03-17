#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <signal.h>
# include <dirent.h>
# include <string.h>
# include <errno.h>
# include <readline/readline.h> // <editline/readline.h>
# include <readline/history.h>
# include "../libft/libft.h"

# define SUCCESS 0
# define ERROR 1

/* Token types */
# define T_WORD 1
# define T_PIPE 2
# define T_REDIR_IN 3
# define T_REDIR_OUT 4
# define T_HEREDOC 5
# define T_APPEND 6
# define T_EOF 7

/* Redirection types */
# define REDIR_IN 1
# define REDIR_OUT 2
# define REDIR_HEREDOC 3
# define REDIR_APPEND 4

typedef struct s_token
{
	char			*value;
	int				type;
	struct s_token	*next;
}	t_token;

typedef struct s_redirect
{
	int					type;
	char				*file;
	struct s_redirect	*next;
}	t_redirect;

typedef struct s_command
{
	char				**args;
	t_redirect			*redirects;
	struct s_command	*next;
	int					pipe_fd[2];
}	t_command;

typedef struct s_shell
{
	char		**env;
	t_command	*commands;
	int			exit_status;
	int			running;
}	t_shell;

/* Global variables */
extern int	g_exec_status; /* 0: interactive, 1: executing command, 2: heredoc */
extern int	g_sigint_received; /* Flag to indicate if SIGINT was received */

/* Main functions */
void	init_shell(t_shell *shell, char **env);
void	run_shell(t_shell *shell);
void	cleanup_shell(t_shell *shell);

/* Parser functions */
t_token		*lexer(char *input);
t_command	*parser(t_token *tokens);
void		expand_variables(t_command *cmd, t_shell *shell);
t_token		*create_token(char *value, int type);
void		add_token(t_token **head, t_token *new_token);
char		*get_word(char *input, int *i);
char		*get_quoted_str(char *input, int *i, char quote);
int			is_operator_char(char c);
int			is_whitespace(char c);

/* Executor functions */
int		execute_commands(t_shell *shell);
int		handle_redirections(t_redirect *redirects);
void	setup_pipes(t_command *cmd);
char	*find_command_path(char *cmd, char **env);
int		execute_builtin(t_command *cmd, t_shell *shell);
int		is_builtin(char *cmd);

/* Builtin functions */
int		ft_echo(char **args);
int		ft_cd(char **args, t_shell *shell);
int		ft_pwd(void);
int		ft_export(char **args, t_shell *shell);
int		ft_unset(char **args, t_shell *shell);
int		ft_env(t_shell *shell);
int		ft_exit(char **args, t_shell *shell);

/* Utils functions */
void	handle_signals(int signum);
void	setup_signals_interactive(void);
void	setup_signals_executing(void);
void	setup_signals_heredoc(void);
void	reset_signals_default(void);
char	*get_env_value(char **env, const char *name);
void	set_env_value(t_shell *shell, const char *name, const char *value);
void	print_error(const char *cmd, const char *msg);

/* Memory management */
void	free_tokens(t_token *tokens);
void	free_commands(t_command *commands);
void	free_array(char **array);

#endif 