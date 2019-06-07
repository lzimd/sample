#include <stdio.h>
#include "invoke.h"

int var = 0x1;
void* pVar = &var;

int main(int argc, const char *argv[])
{
    printf("%p\n", pVar);
    printf("%x\n", var_global);
    fun_global_do();
    printf("%x\n", var_global);
    var_global = 0x1111;
    printf("%x\n", var_global);
    fun_global(var_str, var_global);
    return 0;
}
