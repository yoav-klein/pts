#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

int main(int argc, char** argv)
{
	char buffer[50];
	int read_bytes = 0;
	int fd = 0;
	if(argc < 2)
	{
		printf("Enter pts\n");
		return 1;
	}
	
	fd = open(argv[1], O_RDWR);
	if(fd == -1)
	{
		perror("Couldn't open pts");
		
		return 1;
	}
	
	memset(buffer, 0, 50);
	read_bytes = read(fd, buffer, 50);
	printf("Read bytes: %d\n", read_bytes);
	if(-1 == read_bytes)
	{
		printf("errno: %d\n", errno);
		perror("read");
		
		return 1;
	}
	
	printf("%s\n", buffer);
	printf("read byte: %d\n", buffer[0]);
	
	return 0;
}
