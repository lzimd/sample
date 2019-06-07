as -o main.o main.S
nm main.o
# objdump -d main.o
readelf -s main.o

# main.o:  file size 872 = 0x368
#   [Nr] Name              Type            Address          Off    Size   ES Flg Lk Inf Al
#   [ 0]                   NULL            0000000000000000 000000 000000 00      0   0  0
#   [ 1] .text             PROGBITS        0000000000000000 000040 000020 00  AX  0   0  1
#   [ 2] .rela.text        RELA            0000000000000000 000118 000018 18   I  5   1  8
#   [ 3] .data             PROGBITS        0000000000000000 000060 000000 00  WA  0   0  1
#   [ 4] .bss              NOBITS          0000000000000000 000060 000000 00  WA  0   0  1
#   [ 5] .symtab           SYMTAB          0000000000000000 000060 0000a8 18      6   5  8
#   [ 6] .strtab           STRTAB          0000000000000000 000108 000010 00      0   0  1
#   [ 7] .shstrtab         STRTAB          0000000000000000 000130 000031 00      0   0  1

echo "----"
as -o sum-sub.o sum-sub.S
nm sum-sub.o
# objdump -d sum-sub.o
readelf -s sum-sub.o


# sum-sub.o: file size 720 = 2D0
#   [Nr] Name              Type            Address          Off    Size   ES Flg Lk Inf Al
#   [ 0]                   NULL            0000000000000000 000000 000000 00      0   0  0
#   [ 1] .text             PROGBITS        0000000000000000 000040 00001c 00  AX  0   0  1
#   [ 2] .data             PROGBITS        0000000000000000 00005c 000000 00  WA  0   0  1
#   [ 3] .bss              NOBITS          0000000000000000 00005c 000000 00  WA  0   0  1
#   [ 4] .symtab           SYMTAB          0000000000000000 000060 000078 18      5   4  8
#   [ 5] .strtab           STRTAB          0000000000000000 0000d8 000005 00      0   0  1
#   [ 6] .shstrtab         STRTAB          0000000000000000 0000dd 00002c 00      0   0  1

echo "----"
ld -Ttext=0x0  -o sum.elf main.o sum-sub.o
nm sum.elf
readelf -S -Wl sum.elf
objdump -dx sum.elf

#  [ 1] .text             PROGBITS        0000000000000000 200000 00003c 00  AX  0   0  1

ld -Ttext=0x1000 -o sum.elf main.o sum-sub.o
nm sum.elf
readelf -S -Wl sum.elf
objdump -dx sum.elf

#  [ 1] .text             PROGBITS        0000000000001000 001000 00003c 00  AX  0   0  1

ld -o sum.elf main.o sum-sub.o
nm sum.elf
readelf -S -Wl sum.elf
#  [ 1] .text             PROGBITS        0000000000400078 000078 00003c 00  AX  0   0  1
