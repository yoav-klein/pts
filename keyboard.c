
#include <unistd.h> /* read */
#include <stdio.h> /* perror */
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h> /* open */
#include <linux/input.h> /* struct input_event */



static const char* evval[3] = { "RELEASE", "PRESS",  "REPEAT" };

int main()
{
	const char* dev = "/dev/input/by-path/platform-i8042-serio-0-event-kbd";
	struct input_event ie;
	int bytes_read = 0;
	int fd = 0;
	
	fd = open(dev, O_RDONLY);
	if(-1 == fd) 
	{
		perror("open");
		
		return 1;
	}
	
	while(1)
	{
		bytes_read = read(fd, &ie, sizeof(ie));
		if(-1 == bytes_read)
		{
			perror("read");
			
			return 1;
		}
		
		if(ie.type == EV_KEY && 0 <= ie.value && 2 >= ie.value)
		{
			printf("%s ox%x %d\n", evval[ie.value], ie.code, ie.code);
		}
	}
	
}
