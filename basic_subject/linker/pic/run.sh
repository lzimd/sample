gcc -shared -fPIC -o libfoo.so foo.c

gcc main.c -o prog.out -L. -lfoo

LD_LIBRARY_PATH=$LD_LIBRARY_PATH:./ ./prog.out