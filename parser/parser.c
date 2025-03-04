#include <../includes/parser.h>
// #include "../minishell.h" //// Only if needed for global structures/macros
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function prototypes
// t_ast	*parse_command(char **tokens, int *index);
// t_ast	*parse_pipeline(char **tokens, int *index);
// t_ast	*parse_redirection(char **tokens, int *index);

// Parses redirection
t_ast	*parse_redirection(char **tokens, int *index, t_ast *cmd_node)
{
	t_ast	*redirect_node;

	while (tokens[*index] && (strcmp(tokens[*index], ">") == 0
			|| strcmp(tokens[*index], ">>") == 0 || strcmp(tokens[*index],
				"<") == 0 || strcmp(tokens[*index], "<<") == 0))
	{
		redirect_node = malloc(sizeof(t_ast));
		if (!redirect_node)
		{
			fprintf(stderr, "Memory allocation failed\n");
			return (NULL);
		}
		// Determine the type of redirection
		if (strcmp(tokens[*index], ">") == 0)
		{
			redirect_node->type = NODE_REDIRECT;
		}
		else if (strcmp(tokens[*index], ">>") == 0)
		{
			redirect_node->type = NODE_REDIRECT_APPEND;
			// Define this in your enum
		}
		else if (strcmp(tokens[*index], "<") == 0)
		{
			redirect_node->type = NODE_REDIRECT_INPUT;
		}
		else if (strcmp(tokens[*index], "<<") == 0)
		{
			redirect_node->type = NODE_HEREDOC;
		}
		(*index)++; // Move past the redirection symbol
		if (!tokens[*index])
		{
			fprintf(stderr,
				"Syntax error: expected file name after redirection\n");
			free(redirect_node);
			return (NULL);
		}
		// Store the file name for redirection
		redirect_node->redirect_file = strdup(tokens[*index]);
		(*index)++; // Move past the file name
		// Attach the original command node to the redirection node
		redirect_node->left = cmd_node;
		cmd_node = redirect_node;
		// Update command node to be the redirection node
	}
	return (cmd_node);
}

// Parses a simple command or redirection
t_ast	*parse_command(char **tokens, int *index)
{
	t_ast	*cmd_node;
	int		arg_count;

	if (!tokens[*index])
		return (NULL);
	cmd_node = malloc(sizeof(t_ast));
	cmd_node->type = NODE_CMD;
	cmd_node->args = malloc(10 * sizeof(char *)); // Assume max 10 args
	arg_count = 0;
	while (tokens[*index] && strcmp(tokens[*index], "|") != 0
		&& strcmp(tokens[*index], ">") != 0 && strcmp(tokens[*index], "<") != 0)
	{
		cmd_node->args[arg_count++] = strdup(tokens[*index]);
		(*index)++;
	}
	cmd_node->args[arg_count] = NULL;
	// Call parse_redirection() to check if there are any redirections
	return (parse_redirection(tokens, index, cmd_node));
}

// Parses a pipeline (commands connected with `|`)
t_ast	*parse_pipeline(char **tokens, int *index)
{
	t_ast	*left;
	t_ast	*right;
	t_ast	*pipe_node;

	left = parse_command(tokens, index);
	while (tokens[*index] && strcmp(tokens[*index], "|") == 0)
	{
		(*index)++; // Move past `|`
		right = parse_command(tokens, index);
		pipe_node = malloc(sizeof(t_ast));
		pipe_node->type = NODE_PIPE;
		pipe_node->left = left;
		pipe_node->right = right;
		left = pipe_node; // Chain pipes
	}
	return (left);
}

// Parses a full command line with pipes and redirections
t_ast	*parse_tokens(char **tokens)
{
	int	index;

	index = 0;
	return (parse_pipeline(tokens, &index));
}
// Test function prints AST tree
void	print_ast(t_ast *node, int depth)
{
	if (!node)
		return ;
	// Indentation for better visualization
	for (int i = 0; i < depth; i++)
		printf("  ");
	// Print node type
	if (node->type == NODE_CMD)
	{
		printf("CMD: ");
		for (int i = 0; node->args && node->args[i]; i++)
			printf("%s ", node->args[i]);
		printf("\n");
	}
	else if (node->type == NODE_PIPE)
	{
		printf("PIPE:\n");
	}
	else if (node->type == NODE_REDIRECT)
	{
		printf("REDIRECT (> %s)\n", node->redirect_file);
	}
	else if (node->type == NODE_REDIRECT_APPEND)
	{
		printf("REDIRECT (>> %s)\n", node->redirect_file);
	}
	else if (node->type == NODE_REDIRECT_INPUT)
	{
		printf("REDIRECT (< %s)\n", node->redirect_file);
	}
	else if (node->type == NODE_HEREDOC)
	{
		printf("HEREDOC (<< %s)\n", node->redirect_file);
	}
	// Recursively print left and right nodes (subcommands)
	if (node->left)
		print_ast(node->left, depth + 1);
	if (node->right)
		print_ast(node->right, depth + 1);
}

// function recursively frees all nodes
void	free_ast(t_ast *node)
{
	if (!node)
		return ;
	// Free arguments
	if (node->args)
	{
		for (int i = 0; node->args[i]; i++)
			free(node->args[i]);
		free(node->args);
	}
	// Free redirect filename
	if (node->redirect_file)
		free(node->redirect_file);
	// Recursively free child nodes
	free_ast(node->left);
	free_ast(node->right);
	// Free the node itself
	free(node);
}
