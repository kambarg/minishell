#include "minishell.h"
#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>

int	main(void)
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
}

/*
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
}
*/
