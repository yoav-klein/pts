#include <stdio.h>
#define CTRL(x) (#x[0]-'a'+1)
int main (void)
{
    printf("hello");
    printf("%c", CTRL(n));
    printf("%c", CTRL(z));
}
