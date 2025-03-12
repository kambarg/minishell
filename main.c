#include "minishell.h"
#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>

// ls -l | grep "hello $USER" > output.txt
// cat file.txt | grep hello > output.txt
int	main(void)
{
	char	*input;
	char	**tokens;
	t_ast	*root;

	while (1)
	{
		input = readline("Enter your input message:\n");
		if (!input)
			return (1);
		add_history(input);
			// save input line in history to let user find it with UP key
		if (ft_strncmp("exit", input, 6) == 0)
		{
			free(input);
			exit(0);
		}
		else
		{
			// printf("You entered: %s\n", input);
			tokens = tokenize(input);
			free(input);
			print_tokens(tokens);
			root = parse_tokens(tokens);
			free_tokens(tokens);
			printf("Abstract Syntax Tree:\n");
			print_ast(root, 0);
			free_ast(root);
		}
		free(input);
	}
	return (0);
}

// Main function to demo parser functionality
/* int	main(void)
{
	char	input[] = "cat file.txt | grep hello > output.txt";
	char	**tokens;
	t_ast	*root;

	// Step 1: Tokenize
	tokens = tokenize(input);
	// Step 2: Parse
	root = parse_tokens(tokens);
	// Step 3: Print or Execute the AST (To Be Implemented)
	print_ast(root, 0);
	// Free memory (To Be Implemented)
	free_ast(root);
	return (0);
} */

// Main function to demo lexer functionality
// "ls -l | grep txt > output.txt";
/* int	main(void)
{
	char	*input;

	while (1)
	{
		input = readline("Enter your input message:\n");
		if (!input)
			return (1);
		add_history(input);
			// save input line in history to let user find it with UP key
		if (ft_strncmp("exit", input, 6) == 0)
		{
			free(input);
			exit(0);
		}
		else
			// printf("You entered: %s\n", input);
			lexer(input);
		free(input);
	}
	return (0);
}*/

/* Main function to demo readline functionality
int	main(int argc, char *argv[])
{
		char *res;

	if (argc == 2)
	{
		res = argv[1];
		printf("%s\n", res);
		readline(argv[1]);
	}
	else
		readline("input text string");
	return (0);
}*/
