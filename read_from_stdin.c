
/*************************
*	Try reading from another tty
*	to see what happens
*
*
*
*
*
**********************************/

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>



void read_from_stdin()
{
	int bytes_read = 0;
	int fd = 0;
	char buffer[100];
	
	while(1)
	{
		bytes_read = read(0, buffer, 100);
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

int main(int argc, char** argv)
{
	
	read_from_stdin();

	return 0;
}
