#include <stdio.h>

void do_stuff()
{
    printf("A\n");
}

int main()
{
    for (int i = 0; i < 4; ++i)
    {
        do_stuff();
    }
    printf("hello world!\n");
    return 0;
}