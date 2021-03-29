#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define STDOUT 1
#define STDIN 0

void execute_file();

void execute_file()
{
	int output[2];
	int input[2];
	pid_t pid;
	int status;
	
	if(pipe(output) < 0)
	{
		printf("cannot execute output pipe!");
		return;
	}
	
	if(pipe(input) < 0)
	{
		printf("cannot execute input pipe!");
		return;
	}	
	
	if((pid = fork()) < 0)
	{
		printf("fork error!");
		return;
	}

	//child process
	if(pid == 0)
	{
		dup2(output[1], STDOUT);
		dup2(input[0], STDIN);
		close(output[0]);
		close(input[1]);
		char c[1024];
		int ret = read(input[0], c, sizeof(c));
		write(STDOUT, c, ret);
	}
	else
	{
		close(input[0]);
		close(output[1]);
		int content_size = 6;
		char c[] = "hello123";
		write(input[1], c, sizeof(c) / sizeof(char));
		while(read(output[0], &c, 1) > 0)
			printf("%c\n", *c);
		close(input[1]);
		close(output[0]);
		waitpid(pid, &status, 0);
	}

}

int main()
{
	execute_file();
	return 0;
}
