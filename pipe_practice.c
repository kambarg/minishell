#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>


// int	main(int argc, char **argv)
// {
// 	int	fd[2];
// 	// fd[0] is the read end
// 	// fd[1] is the write end, which is the input end, the left side.
// 	if (pipe(fd) < 0)
// 	{
// 		printf("An error ocurred with opening the pipe\n");
// 		return (1);
// 	}
// 	int	id = fork();
// 	if (id == -1)
// 	{
// 		printf("An error occured with fork\n");
// 		return (2);
// 	}
// 	if (id == 0)
// 	{
// 		close(fd[0]);
// 		int x;
// 		printf("Input a number: ");
// 		scanf("%d", &x);
// 		if (write(fd[1], &x, sizeof(int)) == -1)
// 		{
// 			printf("An error occured with writing to the pipe\n");
// 			return (3);
// 		}
// 		close(fd[1]);
// 	}
// 	else
// 	{
// 		close(fd[1]);
// 		int y;
// 		if(read(fd[0], &y, sizeof(int)) == -1)
// 		{
// 			printf("An error occured whith reading from the pipe\n");
// 			return (4);
// 		}
// 		close(fd[0]);
// 		printf("Got from child process %d\n", y);
// 	}
// 	return(0);
// }

int	main(int argc, char **argv)
{
	int	arr[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9};
	int	arrSize = sizeof(arr) / sizeof(int);
	int	fd[2];
	int	start, end;

	if (pipe(fd) == -1)
		return 1;
	int	id;
	id = fork();
	if (id == -1)
		return 2;
	if (id == 0)
	{
		start = 0;
		end = arrSize / 2;
	}
	else
	{
		start = arrSize / 2;
		end = arrSize;
	}

	int	sum = 0;
	int	i;
	i  = start;
	for (i = start; i < end; i++)
	{
		sum += arr[i];
	}
	printf("Calculated partial sum %d \n", sum);

	if (id == 0)
	{
		if( write(fd[1], &sum, sizeof(sum)) == -1)
			return 3;
		close(fd[1]);
		close(fd[0]);
	}
	else
	{
		int sumFromChild;
		close(fd[1]);
		if (read(fd[0], &sumFromChild, sizeof(sumFromChild)) == -1)
			return 4;
		close(fd[0]);
		printf("sumFromChild %d\n", sumFromChild);
		int	totalSum = sum + sumFromChild;
		printf("Total sum from parent process and child process %d\n", totalSum);
		wait(0);
	}

	return(0);
}
