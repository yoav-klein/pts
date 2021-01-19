#define _XOPEN_SOURCE 
#define _XOPEN_SOURCE_EXTENDED

#include <stdio.h> /* printf */
#include <string.h> /* memset */
#include <unistd.h> /* read */
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	int written_bytes = 0;
	int read_bytes = 0;
	int fd = open("/dev/pts/ptmx", O_RDWR);
	char* name = NULL;
	char write_ctrl_c[2];
	
	printf("fd: %d\n", fd);
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
	
	
	
	/* block until user types something */
	getc(stdin);
	
	write_ctrl_c[0] = 3; /* ASCII code of ctrl+C */
	write_ctrl_c[1] = 10;
	
	written_bytes = write(fd, write_ctrl_c, 2);
	/*written_bytes = write(fd, buffer, read_bytes);*/
	printf("Written bytes: %d\n", written_bytes);
	if(-1 == written_bytes)
	{
		perror("write");
		
		return 1;
	}

	
	return 0;
}
