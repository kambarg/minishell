typedef struct
{
	char *command;
	int count;
} t_node;

void parser(char *str)
{
	char **cmd;
	int i;
	
	cmd = ft_split(str, ';');
	i = 0;
	while (cmd[i])
	{

	}
}
