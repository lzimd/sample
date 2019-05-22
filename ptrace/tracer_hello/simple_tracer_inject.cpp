#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/reg.h>
#include <sys/syscall.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "debuglib.h"

const int long_size = sizeof(uint64_t);

void reverse(char *str)
{
    int i, j;
    char temp;
    for (i = 0, j = strlen(str) - 2; i <= j; ++i, --j)
    {
        temp = str[i];
        str[i] = str[j];
        str[j] = temp;
    }
}

void getdata(pid_t child, uint64_t addr, char *str, int len)
{
    char *laddr;
    int i, j;
    union u {
        uint64_t val;
        char chars[long_size];
    } data;
    i = 0;
    j = len / long_size;
    laddr = str;
    while (i < j)
    {
        data.val = ptrace(PTRACE_PEEKDATA, child, addr + i * long_size, NULL);
        memcpy(laddr, data.chars, long_size);
        ++i;
        laddr += long_size;
    }
    j = len % long_size;
    if (j != 0)
    {
        data.val = ptrace(PTRACE_PEEKDATA, child, addr + i * long_size, NULL);
        memcpy(laddr, data.chars, j);
    }
    str[len] = '\0';
    procmsg("getdata from %p: \n\t %s\n", addr, str);
}

void putdata(pid_t child, long addr, char *str, int len)
{
    char *laddr;
    int i, j;
    union u {
        uint64_t val;
        char chars[long_size];
    } data;
    i = 0;
    j = len / long_size;
    laddr = str;
    while (i < j)
    {
        memcpy(data.chars, laddr, long_size);
        ptrace(PTRACE_POKEDATA, child, addr + i * long_size, data.val);
        ++i;
        laddr += long_size;
    }
    j = len % long_size;
    if (j != 0)
    {
        memcpy(data.chars, laddr, j);
        ptrace(PTRACE_POKEDATA, child, addr + i * long_size, data.val);
    }
    procmsg("putdata to %p: \n\t %s\n", addr, str);
}

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
        int status;
        int toggle = 0;
        while (1)
        {
            wait(&status);
            if (WIFEXITED(status))
                break;
            uint64_t orig_eax = ptrace(PTRACE_PEEKUSER, child, 8 * ORIG_RAX, NULL);
            if (orig_eax == SYS_write)
            {
                if (toggle == 0)
                {
                    toggle = 1;
                    uint64_t params[3];
                    params[0] = ptrace(PTRACE_PEEKUSER, child, 8 * RDI, NULL);
                    params[1] = ptrace(PTRACE_PEEKUSER, child, 8 * RSI, NULL);
                    params[2] = ptrace(PTRACE_PEEKUSER, child, 8 * RDX, NULL);

                    char *str = (char *)calloc((params[2] + 1), sizeof(char));
                    memset(str, 0, params[2] + 1);

                    getdata(child, params[1], str, params[2]);
                    reverse(str);
                    putdata(child, params[1], str, params[2]);
                    free(str);
                }
                else
                {
                    toggle = 0;
                }
            }
            ptrace(PTRACE_SYSCALL, child, NULL, NULL);
        }
    }
    return 0;
}