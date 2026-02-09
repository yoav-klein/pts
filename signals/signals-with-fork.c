#include <stdlib.h> /* exit */
#include <stdio.h>
#include <string.h>
#include <fcntl.h> /* open */
#include <signal.h> /* sigaction */
#include <unistd.h> /* sleep */

int pid = 0;
int fd = 0;

void sigint_handler(int signum)
{
	(void)signum;
    char *message = "GOT SIGNAL";
    write(1, message, strlen(message));

    int res = write(fd, message, strlen(message));
    if(-1 == res) {
        perror("write");
    }

    // Write some text to the file

}

int main() {
    int child = fork();

    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
	sa.sa_handler = sigint_handler;
    sa.sa_flags = SA_RESTART;

    pid = getpid();
    char filename[50];
    sprintf(filename, "file-%d", pid);

    fd = open(filename, O_CREAT | O_WRONLY, 0664);
    if(-1 == fd) {
        perror("failed to open file");
        exit(1);
    }

	if(-1 == sigaction(SIGINT, &sa, NULL))
	{
		perror("sigaction");
		return -1;
	}
    
    while(1) { sleep(1);  }


    return 0;
}
