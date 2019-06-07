# ELF Header

- length: 54 bytes for 32-bit, 64 bytes for 64-bit
- `readelf -h ./prog.out`

## define

```c
typedef struct
{
  unsigned char e_ident[EI_NIDENT];       /* Magic number and other info */
  Elf64_Half    e_type;                   /* Object file type */
  Elf64_Half    e_machine;                /* Architecture */
  Elf64_Word    e_version;                /* Object file version */
  Elf64_Addr    e_entry;                  /* Entry point virtual address */
  Elf64_Off     e_phoff;                  /* Program header table file offset */
  Elf64_Off     e_shoff;                  /* Section header table file offset */
  Elf64_Word    e_flags;                  /* Processor-specific flags */
  Elf64_Half    e_ehsize;                 /* ELF header size in bytes */
  Elf64_Half    e_phentsize;              /* Program header table entry size */
  Elf64_Half    e_phnum;                  /* Program header table entry count */
  Elf64_Half    e_shentsize;              /* Section header table entry size */
  Elf64_Half    e_shnum;                  /* Section header table entry count */
  Elf64_Half    e_shstrndx;               /* Section header string table index */
} Elf64_Ehdr;
```

## example

```hex
00000000: 7F 45 4C 46 02 01 01 00 00 00 00 00 00 00 00 00    .ELF............
00000010: 02 00 3E 00 01 00 00 00 90 04 40 00 00 00 00 00    ..>.......@.....
00000020: 40 00 00 00 00 00 00 00 18 19 00 00 00 00 00 00    @...............
00000030: 00 00 00 00 40 00 38 00 08 00 40 00 1C 00 1B 00    ....@.8...@.....
```

```plain
ELF Header:
  Magic:   7f 45 4c 46 02 01 01 00 00 00 00 00 00 00 00 00
  Class:                             ELF64                                  7F454C46
  Data:                              2's complement, little endian          02
  Version:                           1 (current)                            01
  OS/ABI:                            UNIX - System V                        01
  ABI Version:                       0                                      00
  Type:                              EXEC (Executable file)                 0002
  Machine:                           Advanced Micro Devices X86-64          003E
  Version:                           0x1                                    00000001
  Entry point address:               0x400490                               0000000000400490
  Start of program headers:          64 (bytes into file)                   0000000000000040
  Start of section headers:          6424 (bytes into file)                 0000000000001819
  Flags:                             0x0                                    00000000
  Size of this header:               64 (bytes)                             0040
  Size of program headers:           56 (bytes)                             0038
  Number of program headers:         8                                      0008
  Size of section headers:           64 (bytes)                             0040
  Number of section headers:         28                                     001C
  Section header string table index: 27                                     001B
```

## ident

- magic number
  - 7F 45 4C 46 ( 0x7F ELF )
- classes
  - offset(0x04)
  - 01 32-bit
  - 02 64-bit
- data
  - offset(0x05)
  - 01 LSB (Least Significant Bit)
  - 02 MSB (Most Significant Bit, big-endian)
- version
  - offset(0x06)
  - 0x01
- OS/ABI
  - offset(0x07)
  - often set to 0 regardless of the target platform
- ABI version
  - offset(0x08)
  - further
- Pad

## Type

offset(0x10) len(0x02)

- 0x01   ET_REL   Relocatable file
- 0x02   ET_EXEC  Executable file
- 0x03   ET_DYN   Shared object file
- 0x04   ET_CORE  Core file

## Machine

offset(0x12) len(0x02)

- 0x00   No specific instruction set
- 0x02   SPARC
- 0x03   x86
- 0x14   PowerPC
- 0x16   S390
- 0x28   ARM
- 0x2A   SuperH
- 0x32   IA-64
- 0x3E   x86-64
- 0xB7   AArch64
- 0xF3   RISC-V
- ...

## Version

offset(0x14) len(0x04)

- 0x01, Set to 1 for the original version of ELF

## Entry point address

section .text 的 _start 地址, .init 在 此地址之前
