#define _XOPEN_SOURCE 
#define _XOPEN_SOURCE_EXTENDED

#include <stdio.h> /* printf */
#include <string.h> /* memset */
#include <unistd.h> /* read */
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>

int main()
{
	int written_bytes = 0;
	int read_bytes = 0;
	int fd = open("/dev/pts/ptmx", O_RDWR);
	char* name = NULL;
	char buffer[50];
	printf("%d\n", fd);
	if(-1 == fd)
	{
		perror("Couldn't open file");
		return 1;
	}
	name = ptsname(fd);
	printf("%s\n", name);	
	
	if(-1 == grantpt(fd))
	{
		perror("grantpt");
		
		return 0;
	}
	if(-1 == unlockpt(fd))
	{
		perror("unlockpt");
		
		return 0;
	}
	
	while(1)
	{
		memset(buffer, 0, 50);
		read_bytes = read(0, buffer, 50);
		if(-1 == read_bytes)
		{
			perror("read");
			
			return 1;
		}
		buffer[read_bytes] = 0;
		
		written_bytes = write(fd, buffer, read_bytes);
		if(-1 == written_bytes)
		{
			perror("write");
			
			return 1;
		}
	}
	
	return 0;
}
