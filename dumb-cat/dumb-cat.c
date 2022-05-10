#include <stdio.h> /* printf */
#include <sys/types.h> /* open */
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h> /* exit */
#include <string.h> /* memset */
#include <unistd.h>

int main(int argc, char** argv)
{	
	#define BUFFSIZE (4096)
	char buffer[BUFFSIZE];
	int read_bytes = 0;
	int sid = 0;
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

	/* close current terminal fds */
	close(0);
	close(1);
	close(2);

	/* create new session */
	sid = setsid();
	if(-1 == sid)
	{
		fputs("setsid failed !\n", logger);
		fclose(logger);
		exit(1);
	}

	/* open the terminal as stdin, stdout and stderr */
	if(-1 == open(argv[1], O_RDONLY))
	{
		perror("Couldn't open pts");
		
		return 1;
	}

	if(-1 == open(argv[1], O_WRONLY))
	{
		perror("Couldn't open pts");
		
		return 1;
	}
	
	if(-1 == open(argv[1], O_WRONLY))
	{
		perror("Couldn't open pts");
		
		return 1;
	}

	printf("sid: %d\n", sid);
	perror("sid");
	while(1)
	{
		memset(buffer, 0, sizeof(buffer));
		read_bytes = read(0, buffer, sizeof(buffer));
		printf("Read bytes: %d\n", read_bytes);
		if(-1 == read_bytes)
		{
			perror("read");
			
			return 1;
		}
		printf("%s", buffer);
	}
	
	fclose(logger);

	return 0;
}
