#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/reg.h>
#include <sys/user.h>
#include <sys/syscall.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "debuglib.h"

int main()
{
    pid_t child;
    long orig_rax, rax;
    long params[3] = {0};
    int status;
    int insyscall = 0;
    child = fork();
    if (child == 0)
    {
        ptrace(PTRACE_TRACEME, 0, NULL, NULL);
        execl("/bin/ls", "ls", NULL);
    }
    else
    {
        while (1)
        {
            wait(&status);
            if (WIFEXITED(status))
                break;
            orig_rax = ptrace(PTRACE_PEEKUSER, child, 8 * ORIG_RAX, NULL);
            // printf("the child made a system call %ld\n", orig_rax);
            if (orig_rax == SYS_write)
            {
                if (insyscall == 0)
                {
                    insyscall = 1;
                    params[0] = ptrace(PTRACE_PEEKUSER, child, 8 * RDI, NULL);
                    params[1] = ptrace(PTRACE_PEEKUSER, child, 8 * RSI, NULL);
                    params[2] = ptrace(PTRACE_PEEKUSER, child, 8 * RDX, NULL);
                    procmsg("write called with fd(%llu), buf(0x%016llx), count(%llu)\n", params[0], params[1], params[2]);
                }
                else
                {
                    rax = ptrace(PTRACE_PEEKUSER, child, 8 * RAX, NULL);
                    procmsg("write returned with %ld\n", rax);
                    insyscall = 0;
                }
            }
            ptrace(PTRACE_SYSCALL, child, NULL, NULL);
        }
    }
    return 0;
}