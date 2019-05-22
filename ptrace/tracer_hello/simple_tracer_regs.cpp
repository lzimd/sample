#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/user.h>
#include <sys/reg.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <stdio.h>

#include "debuglib.h"

int main()
{
    pid_t child;
    long orig_rax, rax;
    long params[3] = {0};
    int status = 0;
    int insyscall = 0;
    struct user_regs_struct regs;
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
            if (orig_rax == SYS_write)
            {
                if (insyscall == 0)
                {
                    insyscall = 1;
                    ptrace(PTRACE_GETREGS, child, NULL, &regs);
                    procmsg("write called with fd(%llu), buf(0x%016llx), count(%llu)\n", regs.rdi, regs.rsi, regs.rdx);
                    dump_process_memory(child, regs.rsi, regs.rsi + regs.rdx);
                }
                else
                {
                    ptrace(PTRACE_GETREGS, child, NULL, &regs);
                    procmsg("write returned with %llu\n", regs.rax);
                    insyscall = 0;
                }
            }
            ptrace(PTRACE_SYSCALL, child, NULL, NULL);
        }
    }
    return 0;
}