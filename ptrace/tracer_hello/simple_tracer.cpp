#include <stdio.h>
#include <assert.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <signal.h>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/reg.h>
#include <sys/user.h>
#include <unistd.h>
#include <errno.h>

#include "debuglib.h"

void run_debugger(pid_t child_pid)
{
    int wait_status;
    unsigned icounter = 0;
    procmsg("debugger started\n");

    /* Wait for child to stop on its first instruction */
    wait(&wait_status);

    while (WIFSTOPPED(wait_status))
    {
        icounter++;
        struct user_regs_struct regs;
        ptrace(PTRACE_GETREGS, child_pid, 0, &regs);
        unsigned long instr = ptrace(PTRACE_PEEKTEXT, child_pid, regs.rip, 0);

        procmsg("icounter = %u.  RIP = %p.  instr = 0x%016llx\n",
                icounter, regs.rip, instr);

        /* Make the child execute another instruction */
        if (ptrace(PTRACE_SINGLESTEP, child_pid, 0, 0) < 0)
        {
            perror("ptrace");
            return;
        }

        /* Wait for child to stop on its next instruction */
        wait(&wait_status);
    }

    procmsg("the child executed %u instructions\n", icounter);
}

int main(int argc, char **argv)
{
    pid_t child_pid;

    if (argc < 2)
    {
        fprintf(stderr, "Expected <program name> as argument\n");
        return -1;
    }

    child_pid = fork();
    if (child_pid == 0)
    {
        run_target(argv[1]);
    }
    else if (child_pid > 0)
    {
        run_debugger(child_pid);
    }
    else
    {
        perror("fork");
        return -1;
    }

    return 0;
}