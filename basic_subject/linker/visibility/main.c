#include <stdio.h>

#define PRIVATE __attribute__((visibility("hidden")))
#define PUBLIC __attribute__((visibility("default")))

extern void PRIVATE func_DEFAULT();
extern void PUBLIC func_PROC();
extern void invoke();

int main()
{
    invoke();
    return 0;
}

void func_DEFAULT()
{
    printf("func_DEFAULT redefined in main program, Preempted ==> EXP\n");
}

void func_PROC()
{
    printf("func_PROC redefined in main program, Preempted ==> EXP\n");
}