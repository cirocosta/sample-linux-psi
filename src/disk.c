#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define KiB 1 << 10
#define MiB 1 << 20

/**
 * size of each chunk to be written
 */
#define CHUNK_SIZE 4 * KiB

/**
 * approximate number of MiBs to write
 */
#define MB_TO_WRITE 10

/**
 * directory where the writes will be directed to
 */
#define BASE_DIRECTORY "/tmp"

/**
 * write_to_disk - writes `count` MiB of data to a
 * particular file.
 */
int
write_to_file(int fd, int mebibytes)
{
	char          file_buffer[CHUNK_SIZE] = { 0 };
	unsigned long writes                  = mebibytes * MiB / CHUNK_SIZE;
	int           err, i = writes;

	while (1) {
		memset(file_buffer, rand() % 255, CHUNK_SIZE);

		while (i-- > 0) {
			int n = write(fd, file_buffer, CHUNK_SIZE);
			if (n == -1) {
				perror("write");
				return -1;
			}
		}

		err = fsync(fd);
		if (err == -1) {
			perror("fsync");
			return -1;
		}

		err = lseek(fd, 0, SEEK_SET);
		if (err == -1) {
			perror("lseek");
			return -1;
		}

		i = writes;
	}

	return 0;
}

/**
 *
 */
int
write_to_file_in_directory(char* directory, int mebibytes)
{
	char  filename[512];
	FILE* fp;
	int   fd, err;

	err = snprintf(filename, 512, "%s/file-%d", directory, rand());
	if (err < 0) {
		return -1;
	}

	fp = fopen(filename, "w+");
	if (fp == NULL) {
		perror("fopen");
		return -1;
	}

	err = (fd = fileno(fp));
	if (err == -1) {
		perror("fileno");
		return -1;
	}

	err = write_to_file(fd, mebibytes);
	if (err == -1) {
		printf("failed to write to file");
		return -1;
	}

	return err;
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
					write_to_file_in_directory(
					  BASE_DIRECTORY, MB_TO_WRITE);
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
