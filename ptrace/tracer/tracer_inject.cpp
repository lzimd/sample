#include <sys/ptrace.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/reg.h>
#include <sys/user.h>
#include <errno.h>
#include <signal.h>
#include <assert.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <stdio.h>

#include "debuglib.h"

void checkStatus(int wait_status)
{
    procmsg("  Wait child status: %d\n", wait_status);
    if (WIFEXITED(wait_status)) 
    {
        procmsg("  exited, status: %d\n", WEXITSTATUS(wait_status));
    } 
    else if (WIFSIGNALED(wait_status)) 
    {
        procmsg("  killed by signal %d, %s\n", WTERMSIG(wait_status), strsignal(WTERMSIG(wait_status)));
    } 
    else if (WIFSTOPPED(wait_status)) 
    {
        procmsg("  stopped by signal %d, %s\n", WSTOPSIG(wait_status), strsignal(WSTOPSIG(wait_status)));
    } 
    else if (WIFCONTINUED(wait_status)) 
    {
        procmsg("  continued\n");
    }
}

const int long_size = sizeof(uint64_t);

void dumphex(u_char *str, int len)
{
    int dumpLine = 16;
    int index = 0;
    for (int i=0; i < len; i++)
    {
        if (index == 0)
        {
            printf("        %08d:  ", i);
        }
        printf("%02x ", str[i]);
        index++;
        if (index == dumpLine)
        {
            printf("\n");
            index = 0;
        }
    }
    printf("\n");
}

void getdata(pid_t child, long addr, u_char *str, int len)
{   u_char *laddr;
    int i, j;
    union u {
            long val;
            u_char chars[long_size];
    }data;
    i = 0;
    j = len / long_size;
    laddr = str;
    while(i < j) {
        data.val = ptrace(PTRACE_PEEKDATA, child, addr + i * long_size, NULL);
        memcpy(laddr, data.chars, long_size);
        ++i;
        laddr += long_size;
    }
    j = len % long_size;
    if(j != 0) {
        data.val = ptrace(PTRACE_PEEKDATA, child, addr + i * long_size, NULL);
        memcpy(laddr, data.chars, j);
    }
    str[len] = '\0';
}

void putdata(pid_t child, long addr, u_char *str, int len)
{   u_char *laddr;
    int i, j;
    union u {
            long val;
            u_char chars[long_size];
    }data;
    i = 0;
    j = len / long_size;
    laddr = str;
    while(i < j) {
        memcpy(data.chars, laddr, long_size);
        ptrace(PTRACE_POKEDATA, child,  addr + i * long_size, data.val);
        ++i;
        laddr += long_size;
    }
    j = len % long_size;
    if(j != 0) {
        memcpy(data.chars, laddr, j);
        ptrace(PTRACE_POKEDATA, child,  addr + i * long_size, data.val);
    }
}

int run_inject(pid_t child_pid)
{
    procmsg("injecter started\n");

    /* asm:  nasm -f  elf64 inject.asm & objdump -d inject.o
    section .text
        global main

    main:
        jmp forward

    backward:
        pop rsi
        mov rax, 1
        mov rdi, 1
        mov rdx, 13
        syscall
        int3

    forward:
        call backward

    db "Inject.....",0x0a,0x00
    */ 

    u_char code_inject[] = {
        // <main>
        0xeb,0x13,
        // <backward>
        0x5e,
        0xb8,0x01,0x00,0x00,0x00,
        0xbf,0x01,0x00,0x00,0x00,
        0xba,0x0d,0x00,0x00,0x00,
        0x0f,0x05,
        0xcc,
        // <forward>
        0xe8,0xe8,0xff,0xff,0xff,
        // Inject\n0
        0x49,0x6e,0x6a,0x65,0x63,0x74,0x0a,0x00
    };

    int len_code_inject = sizeof(code_inject) / sizeof(u_char);

    // PTRACE_POKEDATA pid addr data(long)
    int len_padding = long_size - ( len_code_inject % long_size );
    int len = len_code_inject + len_padding;
    u_char backup[len];
    
    struct user_regs_struct regs, newregs;

    int wait_status;
    waitpid(child_pid, &wait_status, 0);
    checkStatus(wait_status);

    ptrace(PTRACE_CONT, child_pid, NULL, NULL);
    procmsg("Child continue, Main sleep 5s\n");

    sleep(5);
    
    procmsg("Will attach child process %d\n", child_pid);
    ptrace(PTRACE_ATTACH, child_pid, NULL, NULL);

    procmsg("Send child signal SIGTRAP\n");
    kill(child_pid, SIGTRAP);

    waitpid(child_pid, &wait_status, 0);
    checkStatus(wait_status);

    procmsg("Attach child process %d\n", child_pid);
    ptrace(PTRACE_GETREGS, child_pid, NULL, &regs);

    // dump_process_memory(child_pid, regs.rip, regs.rip + len + 16);
    getdata(child_pid, regs.rip, backup, len);
    
    procmsg("Inject code\n");
    putdata(child_pid, regs.rip, code_inject, len);
    // dump_process_memory(child_pid, regs.rip, regs.rip + len + 16);

    ptrace(PTRACE_CONT, child_pid, NULL, NULL);

    waitpid(child_pid, &wait_status, 0);
    checkStatus(wait_status);

    procmsg("The process stopped, Putting back the original instructions\n");
    putdata(child_pid, regs.rip, backup, len);
    ptrace(PTRACE_SETREGS, child_pid, NULL, &regs);

    // dump_process_memory(child_pid, regs.rip, regs.rip + len + 16);

    procmsg("Letting it continue with original flow\n");
    int ret = ptrace(PTRACE_DETACH, child_pid, NULL, NULL);
    if ( ret < 0 )
    {
        perror("ERROR:ptrace:PTRACE_DETACH");
    }

    waitpid(child_pid, &wait_status, 0);
    checkStatus(wait_status);
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
        run_inject(child_pid);
    }
    else
    {
        perror("fork");
        return -1;
    }

    return 0;
}