#include <stdio.h>
#include "invoke.h"

int var_global = 0x1234;
char* var_str = "0xxxxxx";

static void fun_static(_IO_FILE *std) {}

void fun_global(char* str, int val)
{
    val += 1;
}

void fun_global_do()
{
    fun_static(stdout);
    var_global = 0x4321;
    fun_global(var_str, var_global);
}
