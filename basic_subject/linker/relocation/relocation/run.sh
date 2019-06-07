gcc -fPIC -c foo.c

gcc -c main.c -o main.o

ar rcs libfoo.a foo.o

gcc -shared -o libfoo.so foo.o

gcc -no-pie main.o foo.o -o prog.out
gcc -no-pie main.o -L. -Wl,-Bstatic -lfoo -Wl,-Bdynamic -o prog_s.out
gcc -no-pie main.o -L. -Wl,-Bstatic -Wl,-Bdynamic -lfoo -L. -o prog_d.out

file *.a
file *.so
file *.out

readelf -r *.so
readelf -r *.out

LD_LIBRARY_PATH=$LD_LIBRARY_PATH:./ ./prog_d.out
