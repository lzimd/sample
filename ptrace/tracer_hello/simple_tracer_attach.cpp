#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/user.h>
#include <sys/reg.h>
#include <unistd.h>
#include <stdio.h>
#include <stdint.h>

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
        printf("child pid : %d\n", child);
        ptrace(PTRACE_ATTACH, child, NULL, NULL);
        wait(NULL);
        struct user_regs_struct regs;
        ptrace(PTRACE_GETREGS, child, NULL, &regs);
        uint64_t ins = ptrace(PTRACE_PEEKTEXT, child, regs.rip, NULL);
        printf("RIP: %p Instruction executed: 0x%016lx\n", (void *)regs.rip, ins);
        ptrace(PTRACE_DETACH, child, NULL, NULL);
        wait(NULL);
    }
    return 0;
}