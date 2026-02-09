#include<stdio.h>
#include <signal.h> /* sigaction */
#include <unistd.h> /* sleep */

void sigint_handler(int signum)
{
	(void)signum;

	printf("Oh, got that %d hard!", signum);

}

int main() {
    struct sigaction sa;
	sa.sa_handler = sigint_handler;

	if(-1 == sigaction(SIGINT, &sa, NULL))
	{
		perror("sigaction");
		return -1;
	}
    
    while(1) { sleep(1);  }

    return 0;
}
