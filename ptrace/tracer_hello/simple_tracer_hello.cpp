#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/reg.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>

int main()
{
    pid_t child;
    child = fork();
    if (child == 0)
    {
        ptrace(PTRACE_TRACEME, 0, NULL, NULL);
        execl("/bin/ls", "ls", NULL);
    }
    else
    {
        wait(NULL);
        unsigned long orig_rax = ptrace(PTRACE_PEEKUSER, child, 8 * ORIG_RAX, NULL);
        printf("The child made a system call 0x%lx\n", orig_rax);
        ptrace(PTRACE_CONT, child, NULL, NULL);
        wait(NULL);
    }
    return 0;
}