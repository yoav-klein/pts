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
#include <sys/epoll.h> /* epoll */

int set_terminal()
{
	struct termios tty;

	if(-1 == tcgetattr(0, &tty))
	{
		perror("tcgetattr");
		return -1;
	}

	tty.c_lflag &= ~ECHO;
	tty.c_lflag &= ~ISIG;
	tty.c_lflag &= ~ICANON;

	if(-1 == tcsetattr(0, TCSANOW, &tty))
	{
		perror("tcsetattr");
		return -1;
	}

	return 0;

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
	
	if(-1 == grantpt(master_fd))
	{
		perror("grantpt");
		
		return -1;
	}
	if(-1 == unlockpt(master_fd))
	{
		perror("unlockpt");
		
		return -1;
	}

	return master_fd;
}

int configure_epoll(int master_fd)
{
	struct epoll_event stdin_event = { 0 };
	struct epoll_event master_event = { 0 };
	
	int epoll_fd = 0;
	
	epoll_fd = epoll_create1(0); /* 0 is the flags parameter */
	if(-1 == epoll_fd)
	{
		perror("epoll_create1");
		return -1;	
	}
	
	stdin_event.events = EPOLLIN | EPOLLRDHUP | EPOLLHUP;
	stdin_event.data.fd = 0;
	if(-1 == epoll_ctl(epoll_fd, EPOLL_CTL_ADD, 0, &stdin_event))
	{
		perror("epoll_ctl");
	}
	
	master_event.events = EPOLLIN | EPOLLRDHUP | EPOLLHUP;
	master_event.data.fd = master_fd;
	if(-1 == epoll_ctl(epoll_fd, EPOLL_CTL_ADD, master_fd, &master_event))
	{
		perror("epoll_ctl");
		return -1;
	}
	
	return epoll_fd;
}

int talk_to_application(int epoll_fd, int master_fd)
{
	#define MAX_EVENTS (10)
	#define BUFFSIZE (4096)
	struct epoll_event events[MAX_EVENTS];
	int nfds = 0;
	int read_bytes = 0, written_bytes = 0;
	int i = 0;
	int stop = 0;
	char buffer[BUFFSIZE];

	while(!stop)
	{	
		nfds = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
		if(-1 == nfds)
		{
			perror("epoll_wait");
			exit(1);
		}

		for(i = 0; i < nfds; ++i)
		{
			if(0 == events[i].data.fd) /* stdin */
			{
				read_bytes = read(0, buffer, sizeof(buffer));
				if(-1 == read_bytes)
				{
					perror("read from stdin");
					return -1;
				}

				written_bytes = write(master_fd, buffer, read_bytes);
				if(-1 == written_bytes)
				{
					perror("write to application");
					return -1;
				}
			}
			if(master_fd == events[i].data.fd) /* application */
			{
				if(events[i].events & EPOLLHUP)
				{
					stop = 1;
				}
				if(events[i].events & EPOLLIN)
				{
					read_bytes = read(master_fd, buffer, sizeof(buffer));
					if(-1 == read_bytes)
					{
						perror("read from application");
						return -1;
					}
					written_bytes = write(1, buffer, read_bytes);
					if(-1 == written_bytes)
					{
						perror("write to stdout");
						return -1;
					}
				}
				
			}
		}
	}
	
	return 0;
}

int main()
{
	char *terminal_name = NULL;
	char cwd[150];
	char dumb_cat_path[160];
	int master_fd = 0;
	int epoll_fd = 0;
	int pid = 0;

	master_fd = create_new_terminal();
	if(-1 == master_fd)
	{
		printf("couldn't create new terminal\n");
		exit(1);
	}

	terminal_name = ptsname(master_fd);

	/* fork the child process and execute dumb-cat */
	pid = fork();
	if(-1 == pid)
	{
		perror("fork");
		exit(1);
	}

	if(0 == pid) /* child */
	{
		getcwd(cwd, sizeof(cwd));
		sprintf(dumb_cat_path, "%s/dumb-cat", cwd);
		execl(dumb_cat_path, "dumb-cat", terminal_name, NULL);

		return 0;
	}
	
	/* set current terminal to raw mode */
	if(-1 == set_terminal())
	{
		printf("couldn't set terminal\n");
		exit(1);
	}

	/* configure epoll */
	epoll_fd = configure_epoll(master_fd);
	if(-1 == master_fd)
	{
		printf("couldn't configure epoll");
		exit(1);
	}

	/* talk to dumb-cat */
	if(-1 == talk_to_application(epoll_fd, master_fd))
	{
		printf("failed talking to application\n");
		exit(1);
	}
	
	printf("Done!\n");
	return 0;
}
