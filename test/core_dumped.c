#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
	pid_t pid = fork();

	if (pid == 0)
		raise(SIGQUIT);
	else
	{
		int status;
		waitpid(pid, &status, 0);
		if (WIFSIGNALED(status) && WTERMSIG(status) == SIGQUIT) {
			printf("Quit (core dumped)\n");
		}
	}

	return 0;
}

