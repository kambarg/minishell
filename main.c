#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t			i;
	unsigned char	*ss1;
	unsigned char	*ss2;

	i = 0;
	ss1 = (unsigned char *)s1;
	ss2 = (unsigned char *)s2;
	while (ss1[i] != '\0' && ss2[i] != '\0' && i < n)
	{
		if (ss1[i] - ss2[i] == 0)
			i++;
		else
			return (ss1[i] - ss2[i]);
	}
	if (ss1[i] == '\0' && ss2[i] != '\0' && i < n)
		return (-1 * ss2[i]);
	else if (ss1[i] != '\0' && ss2[i] == '\0' && i < n)
		return (ss1[i]);
	else
		return (0);
}

int main(void)
{
	char *input;
	
	while (1)
	{
		input = readline("Enter your input message:\n");
		if (!input)
			return (1);
		add_history(input); // save input line in history to let user find it with UP key
		if (ft_strncmp("exit", input, 6) == 0)
		{			
			free(input);
			exit (0);
		}	
		else
			printf("You entered: %s\n", input);
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
