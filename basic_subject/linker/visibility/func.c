#include <stdio.h>
void func_DEFAULT()
{
    printf("func_DEFAULT in the shared library, Not preempted\n");
}

void func_PROC()
{
    printf("func_PROC in the shared library, Not preempted\n");
}