#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/**
 * Description:
 *   Loads CPUS with a bunch of usage.
 *
 * Usage
 *   cpu <number of processes to fork>
 *
 */

void
start_workers(int n)
{
	int pid;

	while (n--) {
		switch (pid = fork()) {
			case -1:
				perror("fork:");
				exit(1);
			case 0:
				printf("%d\n", n);
				while (1) {
				}
				exit(0);
		}
	}
}

int
main(int argc, char** argv)
{
	if (argc != 2) {
		fprintf(stderr, "Usage: cpu $number_of_procs_to_fork\n");
		exit(1);
	}

	setbuf(stdout, NULL);
	setbuf(stderr, NULL);

	start_workers(atoi(argv[1]));
	pause();

	return 0;
}
