#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
	int fd;
	fd = open(argv[1], O_RDWR|O_CREAT|O_TRUNC, 0600);
	ftruncate(fd, strtoull(argv[2], NULL, 0));
	close(fd);
	return 0;
}
