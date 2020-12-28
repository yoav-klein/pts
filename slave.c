#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>

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
	
	while(1)
	{
		memset(buffer, 0, 50);
		read_bytes = read(fd, buffer, 50);
		if(-1 == read_bytes)
		{
			perror("read");
			
			return 1;
		}
		printf("%s\n", buffer);
	}
	
	return 0;
}
