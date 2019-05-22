#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
    pid_t pid = getpid();
    uid_t uid = getuid();
    int i;
    for (i = 0; i < 5; ++i)
    {
        printf("[%d] counter: %d\n", pid, i);
        sleep(2);
    }
    return 0;
}