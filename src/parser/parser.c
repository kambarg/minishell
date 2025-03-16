#include "../../includes/minishell.h"

static t_redirect	*create_redirect(int type, char *file)
{
	t_redirect	*redir;

	redir = (t_redirect *)malloc(sizeof(t_redirect));
	if (!redir)
		return (NULL);
	redir->type = type;
	redir->file = ft_strdup(file);
	redir->next = NULL;
	return (redir);
}

static void	add_redirect(t_command *cmd, t_redirect *new_redir)
{
	t_redirect	*current;

	if (!cmd->redirects)
	{
		cmd->redirects = new_redir;
		return ;
	}
	current = cmd->redirects;
	while (current->next)
		current = current->next;
	current->next = new_redir;
}

static t_command	*create_command(void)
{
	t_command	*cmd;

	cmd = (t_command *)malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	cmd->args = NULL;
	cmd->redirects = NULL;
	cmd->next = NULL;
	cmd->pipe_fd[0] = -1;
	cmd->pipe_fd[1] = -1;
	return (cmd);
}

static void	add_command(t_command **head, t_command *new_cmd)
{
	t_command	*current;

	if (!*head)
	{
		*head = new_cmd;
		return ;
	}
	current = *head;
	while (current->next)
		current = current->next;
	current->next = new_cmd;
}

static int	add_argument(t_command *cmd, char *arg)
{
	int		i;
	int		size;
	char	**new_args;

	size = 0;
	if (cmd->args)
		while (cmd->args[size])
			size++;
	new_args = (char **)malloc(sizeof(char *) * (size + 2));
	if (!new_args)
		return (0);
	i = 0;
	while (i < size)
	{
		new_args[i] = cmd->args[i];
		i++;
	}
	new_args[i] = ft_strdup(arg);
	new_args[i + 1] = NULL;
	free(cmd->args);
	cmd->args = new_args;
	return (1);
}

static int	handle_redirect(t_token **token, t_command *cmd)
{
	int		type;
	char	*file;

	if ((*token)->type == T_REDIR_IN)
		type = REDIR_IN;
	else if ((*token)->type == T_REDIR_OUT)
		type = REDIR_OUT;
	else if ((*token)->type == T_HEREDOC)
		type = REDIR_HEREDOC;
	else
		type = REDIR_APPEND;
	*token = (*token)->next;
	if (!*token || (*token)->type != T_WORD)
	{
		print_error(NULL, "syntax error near unexpected token");
		return (0);
	}
	file = (*token)->value;
	add_redirect(cmd, create_redirect(type, file));
	return (1);
}

t_command	*parser(t_token *tokens)
{
	t_command	*commands;
	t_command	*current;

	commands = NULL;
	current = create_command();
	while (tokens)
	{
		if (tokens->type == T_WORD)
		{
			if (!add_argument(current, tokens->value))
				return (NULL);
		}
		else if (tokens->type == T_PIPE)
		{
			add_command(&commands, current);
			current = create_command();
		}
		else if (tokens->type >= T_REDIR_IN && tokens->type <= T_APPEND)
		{
			if (!handle_redirect(&tokens, current))
				return (NULL);
		}
		tokens = tokens->next;
	}
	add_command(&commands, current);
	return (commands);
} 