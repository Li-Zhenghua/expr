#include <stdlib.h>
#include <unistd.h>

int main()
{
	int fd[2];
	const int BSIZE = 100;
	char buf[BSIZE];
	ssize_t nbytes;
	int status;
	int pid;
	status = pipe(fd);

	switch (fork())
	{
	case -1:
		break;
	case 0:
		close(fd[0]);
		write(fd[1], "Child process 1 is sending a message!\n", 38);
		close(fd[1]);
		exit(EXIT_SUCCESS);
	default:
		wait();
		switch (fork())
		{
		case -1:
			break;
		case 0:
			close(fd[0]);
			write(fd[1], "Child process 2 is sending a message!", 38);
			close(fd[1]);
			exit(EXIT_SUCCESS);
		default:
			wait();
			close(fd[1]);
			nbytes = read(fd[0], buf, BSIZE);
			close(fd[0]);
			printf("%s", buf);
			exit(EXIT_SUCCESS);
		}
	}
}
