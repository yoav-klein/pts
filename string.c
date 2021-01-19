
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
	char str[50];
	int read_bytes = 0;
	int i = 0;
	
	memset(str, 0, 50);
	read_bytes = read(0, str, 50);
	if(-1 == read_bytes)
	{
		perror("read");
	}
	
	for(i = 0; i < strlen(str); ++i)
	{
		printf("%d ", str[i]);
	}	
	
	return 0;
}
