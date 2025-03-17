#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <editline/readline.h>
#include <sys/wait.h>

// #include <readline/readline.h>
// #include <readline/history.h>

int main(void)
{
    char *rl;
    rl = readline("Prompt > ");
    printf("%s\n", rl);

	// fork()
	// {
    char *pwd;
    
    pwd = getcwd(NULL, 0);
    printf("pwd: %s\n", pwd);
    printf("pwd before chdir: %s\n", pwd);
  // chdir("/Users/rraidan/Desktop/My_Cursus/8_minishell/hi");
    pwd = getcwd(NULL, 0);
    printf("pwd after chdir: %s\n", pwd);

	char	*argv[] = {getcwd(NULL, 0), NULL, NULL};
	int		val;

	val = 0;
	val = execve(argv[0], argv, NULL);
	if (val == -1)
		perror("Error");
	printf("Done with exersize\n");
	// }
	free(rl);
    return (0);
}

void	cmd_execute()
{
	// save standard input/output file descriptors to temp variables
	int	tmpin = dup(0);
	int	tmpout = dup(1);

	// set the initial input
	int	fdin;
	
}