#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>


#define KiB 1 << 10
#define MiB 1 << 20
#define CHUNK_SIZE 4 * KiB

/**
 *
 */
int
write_to_file_in_directory(char directory, int mebibytes)
{
	char* filename;
	FILE* fp;
	int fd, err;

	// TODO check err
	int err = asprintf(&filename, "%s/file-%d", directory, rand());

	// TODO check err
	err = (fp = fopen(filename, "w+"));

	// TODO check err
	err = (fd = fileno(fp));

	// TODO check err
	err = write_to_file(fd, mebibytes);

	return err;
}

/**
 * write_to_disk - writes `count` MiB of data to a
 * particular file.
 */
void
write_to_file(int fd, int mebibytes)
{
	char file_buffer[CHUNK_SIZE] = { 0 };
	unsigned long writes = mebibytes * MiB / CHUNK_SIZE;
	int i = writes;

	while (1) {
		// TODO check err w/ unlikely
		memset(file_buffer, rand() % 255, CHUNK_SIZE);

		while (i-- > 0) {
			_STRESS_MUST(
			  write(fd, file_buffer, CHUNK_SIZE) != -1,
			  "Couldn't properly write chunk to file %s (fd=%d)",
			  filename,
			  fd);
		}

		_STRESS_MUST(
		  fsync(fd) != -1,
		  "Couldn't sync filedescriptor with disk (fname=%s)",
		  filename);
		_STRESS_MUST(lseek(fd, 0, SEEK_SET) != -1,
		             "Couldn't seek to init of file %s",
		             filename);
		i = writes;
	}

	return NULL;
}

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
					write_to_disk("/mnt/slow/" +
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

	srand(time(NULL));
	setbuf(stdout, NULL);
	setbuf(stderr, NULL);

	start_workers(atoi(argv[1]));
	pause();

	return 0;
}
