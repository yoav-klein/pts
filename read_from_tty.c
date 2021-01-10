
/*************************
*	Try reading from another tty
*	to see what happens
*
*
*
*
*
**********************************8/

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>


int read_from_tty(char *tty)
{
	int bytes_read = 0;
	int fd = 0;
	char buffer[100];
	
	fd = open(tty, O_RDWR);
	if(-1 == fd)
	{
		printf("Couldn't open pts\n");
		
		return 1;
	}
	printf("File opened: %d\n", fd);
	
	while(1)
	{
		bytes_read = read(fd, buffer, 100);
		if(-1 == bytes_read)
		{
			perror("read");
		}
		if(-1 == write(1, buffer, bytes_read))
		{
			perror("write:");
		}
		memset(buffer, 0, 100);
	}
}

int write_to_tty(char* tty)
{
	int bytes_read = 0;
	int fd = 0;
	char buffer[100];
	
	fd = open(tty, O_RDWR);
	if(-1 == fd)
	{
		printf("Couldn't open pts\n");
		
		return 1;
	}
	printf("File opened: %d\n", fd);
	while(1)
	{
		bytes_read = read(0, buffer, 100);
		if(-1 == bytes_read)
		{
			perror("read");
		}
		if(-1 == write(fd, buffer, bytes_read))
		{
			perror("write:");
		}
		memset(buffer, 0, 100);
	
	}
}	

int main(int argc, char** argv)
{
	if(argc < 2)
	{
		printf("enter path of pts\n");
		return 1;
	}
	
	read_from_tty(argv[1]);

	return 0;
}
