cpp main.c -o tmp/main.i
/usr/lib/gcc/x86_64-linux-gnu/7/cc1 -quiet tmp/main.i -Og -o tmp/main.S
as tmp/main.S -o tmp/main.o

gcc -E sum.c -o tmp/sum.i
gcc -S tmp/sum.i -Og -o tmp/sum.S
gcc -c tmp/sum.S -o tmp/sum.o

# -pie -z now -z relro 
ld -o prog.out \
  -m elf_x86_64 \
  -dynamic-linker /lib64/ld-linux-x86-64.so.2\
  /usr/lib/x86_64-linux-gnu/Scrt1.o \
  /usr/lib/x86_64-linux-gnu/crti.o \
  /usr/lib/gcc/x86_64-linux-gnu/7/crtbeginS.o \
  tmp/main.o tmp/sum.o \
  -lc \
  /usr/lib/gcc/x86_64-linux-gnu/7/crtendS.o \
  /usr/lib/x86_64-linux-gnu/crtn.o

file ./prog.out

./prog.out
echo $?

readelf -a ./prog.out > prog.elf
