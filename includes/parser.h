#ifndef PARSER_H
# define PARSER_H

# include "../libft/libft.h"
# include "lexer.h"

typedef enum e_ast_type 
{
    NODE_CMD,            // Command node (e.g., "ls -l")
    NODE_PIPE,           // Pipe node (e.g., "ls | grep txt")
    NODE_REDIRECT,       // Redirection (>)
    NODE_REDIRECT_APPEND, // Redirection (>>)
    NODE_REDIRECT_INPUT,  // Redirection (<)
    NODE_HEREDOC         // Heredoc (<<)
} t_ast_type;

typedef struct s_ast
{
    t_ast_type type;      // Type of the node
    char **args;          // Arguments for CMD nodes (e.g., ["ls", "-l"])
    char *redirect_file;  // File for redirections (e.g., "output.txt" for "> output.txt")
    
    struct s_ast *left;   // Left child (for pipes, redirections)
    struct s_ast *right;  // Right child (for pipes, redirections)
} t_ast;


t_ast	*parse_tokens(char **tokens); // Parses a full command line with pipes and redirections
void	print_ast(t_ast *node, int depth); // Test function prints AST tree
void	free_ast(t_ast *node);             // Recursively frees all nodes

#endif