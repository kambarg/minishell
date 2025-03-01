#ifndef PARSER_H
# define PARSER_H

# include "../libft/libft.h"
# include "lexer.h"

typedef enum e_node_type {
    NODE_CMD,    // Simple command (e.g., "ls -l")
    NODE_PIPE,   // Pipe (|)
    NODE_REDIRECT // Redirection (> or <)
} t_node_type;

typedef struct s_ast {
    t_node_type type;
    char **args;          // Command arguments (if NODE_CMD)
    char *redirect_file;  // File for redirection (if NODE_REDIRECT)
    struct s_ast *left;   // Left subtree (e.g., first command in a pipe)
    struct s_ast *right;  // Right subtree (e.g., second command in a pipe)
} t_ast;

#endif