gcc -c func.c invoke.c -fpic

# --retain-symbols-file
# --version-script
gcc  -shared -o libtest.so func.o invoke.o -Wl,--version-script=exports.map

gcc -c main.c

# enable runtime linkage
gcc main.o -L. -ltest -o main.out

readelf -a libtest.so > libtest.elf
readelf -a main.out > main.elf

LD_LIBRARY_PATH=$LD_LIBRARY_PATH:./ ./main.out
