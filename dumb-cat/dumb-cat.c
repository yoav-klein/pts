#define _POSIX_C_SOURCE (1)

#include <stdio.h> /* printf */
#include <sys/types.h> /* open */
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h> /* exit */
#include <string.h> /* memset */
#include <unistd.h>
#include <signal.h> /* sigaction */
#include <errno.h> /* errno */

int counter = 3;

void sigint_handler(int signum)
{
	(void)signum;

	printf("Ouchh.. %d more to go\n", --counter);
	if(0 == counter)
	{
		printf("exiting !\n");
		exit(1);
	}

}

int set_sighandler()
{
	struct sigaction sa;
	sa.sa_handler = sigint_handler;

	if(-1 == sigaction(SIGINT, &sa, NULL))
	{
		perror("sigaction");
		return -1;
	}

	return 0;
}

int open_new_terminal(const char *device, FILE *logger)
{
	int sid = 0;
	/* create new session */
	sid = setsid();
	if(-1 == sid)
	{
		fputs("setsid failed !\n", logger);
		fclose(logger);
		exit(1);
	}

	/* open the terminal as stdin, stdout and stderr */
	if(-1 == open(device, O_RDONLY))
	{
		fputs(strerror(errno), logger);
		
		return -1;
	}

	if(-1 == open(device, O_WRONLY))
	{
		fputs(strerror(errno), logger);
		
		return -1;
	}
	
	if(-1 == open(device, O_WRONLY))
	{
		fputs(strerror(errno), logger);
		
		return -1;
	}

	return 0;
}

int main(int argc, char** argv)
{	
	#define BUFFSIZE (4096)
	char buffer[BUFFSIZE];
	int read_bytes = 0;
	
	FILE *logger;
	
	if(argc < 2)
	{
		printf("Enter pts\n");
		return 1;
	}
	
	logger = fopen("dumb-cat.log", "w");
	if(NULL == logger)
	{
		printf("couldn't create logger file\n");
		perror("fopen");
		exit(1);
	}

	printf("opening pts: %s\n", argv[1]);

	if(-1 == set_sighandler())
	{
		printf("couldn't set sighandler\n");
		exit(1);
	}

	/* close current terminal fds */
	close(0);
	close(1);
	close(2);

	if(-1 == open_new_terminal(argv[1], logger))
	{
		fputs("couldn't open new terminal", logger);
		exit(1);
	}

	while(1)
	{
		memset(buffer, 0, sizeof(buffer));
		read_bytes = read(0, buffer, sizeof(buffer));
		if(-1 == read_bytes)
		{
			if(errno == EINTR)
			{
				continue;
			}
			perror("read");
			
			return 1;
		}
		printf("%s", buffer);
	}
	
	fclose(logger);

	return 0;
}
