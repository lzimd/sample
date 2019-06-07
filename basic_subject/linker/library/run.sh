gcc -c ff.c -o tmp/ff.o
gcc -c test.c -o tmp/test.o
gcc -c main.c -o tmp/main.o

ld -o prog.out \
  -m elf_x86_64 \
  -dynamic-linker /lib64/ld-linux-x86-64.so.2\
  /usr/lib/x86_64-linux-gnu/Scrt1.o \
  /usr/lib/x86_64-linux-gnu/crti.o \
  /usr/lib/gcc/x86_64-linux-gnu/7/crtbeginS.o \
  tmp/ff.o tmp/test.o tmp/main.o \
  -lc \
  /usr/lib/gcc/x86_64-linux-gnu/7/crtendS.o \
  /usr/lib/x86_64-linux-gnu/crtn.o

# 多个 .o 合并成一个 .o
ld -r tmp/ff.o tmp/test.o -o tmp/libtest.o
ld -o prog_o.out \
  -m elf_x86_64 \
  -dynamic-linker /lib64/ld-linux-x86-64.so.2\
  /usr/lib/x86_64-linux-gnu/Scrt1.o \
  /usr/lib/x86_64-linux-gnu/crti.o \
  /usr/lib/gcc/x86_64-linux-gnu/7/crtbeginS.o \
  tmp/libtest.o tmp/main.o \
  -lc \
  /usr/lib/gcc/x86_64-linux-gnu/7/crtendS.o \
  /usr/lib/x86_64-linux-gnu/crtn.o

# static library
ar rcs tmp/libtest.a tmp/ff.o tmp/test.o
nm tmp/libtest.a 
ld -o prog_a.out \
  -m elf_x86_64 \
  -dynamic-linker /lib64/ld-linux-x86-64.so.2\
  /usr/lib/x86_64-linux-gnu/Scrt1.o \
  /usr/lib/x86_64-linux-gnu/crti.o \
  /usr/lib/gcc/x86_64-linux-gnu/7/crtbeginS.o \
  tmp/main.o \
  -lc \
  /usr/lib/gcc/x86_64-linux-gnu/7/crtendS.o \
  /usr/lib/x86_64-linux-gnu/crtn.o \
  -Bstatic -L ./tmp -l test  \

# shared library
ld -shared tmp/ff.o tmp/test.o -o tmp/libtest.so
ld -o prog_so.out \
  -m elf_x86_64 \
  -dynamic-linker /lib64/ld-linux-x86-64.so.2\
  /usr/lib/x86_64-linux-gnu/Scrt1.o \
  /usr/lib/x86_64-linux-gnu/crti.o \
  /usr/lib/gcc/x86_64-linux-gnu/7/crtbeginS.o \
  tmp/main.o \
  -lc \
  /usr/lib/gcc/x86_64-linux-gnu/7/crtendS.o \
  /usr/lib/x86_64-linux-gnu/crtn.o \
  -Bdynamic -L ./tmp -l test  \

tree .