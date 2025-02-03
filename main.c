#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>

int main(void)
{
	char *input;
	
	while (1)
	{
		input = readline("enter your input message\n");
		if (!input)
			break;
		printf("You entered %s\n", input);
		free(input);
	}
	return (0);
}

/*
int main(int argc, char *argv[])
{
	if (argc == 2)
	{
		char *res;

		res = argv[1];
		printf("%s\n", res);
		readline(argv[1]);
	}
	else
		readline("input text string");
	return (0);
}
*/
