#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/user.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <stdio.h>
#include "debuglib.h"

int main(int argc, char **argv)
{
    pid_t child;
    const int long_size = sizeof(uint64_t);
    if (argc < 2)
    {
        fprintf(stderr, "Expected <program name> as argument\n");
        return -1;
    }

    child = fork();
    if (child == 0)
    {
         run_target(argv[1]);
    }
    else
    {
        int status;
        union u {
            uint64_t val;
            char chars[long_size];
        } data;
        int start = 0;
        while (1)
        {
            wait(&status);
            if (WIFEXITED(status))
                break;
            struct user_regs_struct regs;
            ptrace(PTRACE_GETREGS, child, NULL, &regs);

            uint64_t ins = ptrace(PTRACE_PEEKTEXT, child, regs.rip, NULL);
            procmsg("RIP: %llx Instruction executed: %llx\n", regs.rip, ins);

            if (regs.orig_rax == SYS_write)
            {
                ptrace(PTRACE_SINGLESTEP, child, NULL, NULL);
            }
            else
            {
                ptrace(PTRACE_SYSCALL, child, NULL, NULL);
            }
        }
    }
    return 0;
}