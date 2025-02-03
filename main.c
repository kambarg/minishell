#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>


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
