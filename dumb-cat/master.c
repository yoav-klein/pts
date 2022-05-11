#define _XOPEN_SOURCE 
#define _XOPEN_SOURCE_EXTENDED

#include <stdio.h> /* printf */
#include <string.h> /* memset */
#include <unistd.h> /* read */
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h> /* exit */
#include <termios.h> /* struct termios */

void set_terminal()
{
	struct termios tty;

	if(-1 == tcgetattr(0, &fd))
	{
		perror("tcgetattr");
		exit(1);
	}


}


int create_new_terminal()
{
	int master_fd = 0;
	
	/* create new terminal */
	master_fd = open("/dev/ptmx", O_RDWR);
	if(-1 == master_fd)
	{
		perror("Couldn't open file");
		return -1;
	}
	
	if(-1 == grantpt(fd))
	{
		perror("grantpt");
		
		return -1;
	}
	if(-1 == unlockpt(fd))
	{
		perror("unlockpt");
		
		return -1;
	}

	return master_fd
}



int main()
{
	char *terminal_name = NULL;
	#define BUFFSIZE (4096)
	int written_bytes = 0;
	int read_bytes = 0;
	int master_fd = 0;
	int epoll_fd = 0;

	master_fd = create_new_terminal();
	if(-1 == matser_fd)
	{
		printf("couldn't create new terminal\n");
		exit(1);
	}

	terminal_name = ptsname(fd)

	/* fork the child process and execute dumb-cat */


	/* set current terminal to raw mode */
	set_terminal();

	/* configure epoll */

	/* talk to dumb-cat */
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
