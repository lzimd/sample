# ELF section

- `readelf -S ./prog.out`

## define

```c
typedef struct
{
  Elf64_Word     sh_name;          /* Section name (string tbl index) */
  Elf64_Word     sh_type;          /* Section type */
  Elf64_Xword    sh_flags;         /* Section flags */
  Elf64_Addr     sh_addr;          /* Section virtual addr at execution */
  Elf64_Off      sh_offset;        /* Section file offset */
  Elf64_Xword    sh_size;          /* Section size in bytes */
  Elf64_Word     sh_link;          /* Link to another section */
  Elf64_Word     sh_info;          /* Additional section information */
  Elf64_Xword    sh_addralign;     /* Section alignment */
  Elf64_Xword    sh_entsize;       /* Entry size if section holds table */
} Elf64_Shdr;
```

## section header

- Type
  - PROGBITS       1  /* Program data */
  - SYMTAB         2  /* Symbol table */
  - DYNSYM        11  /* Dynamic linker symbol table */
  - STRTAB         3  /* String table */
  - REL            9  /* Relocation entries, no addends */
  - RELA           4  /* Relocation entries with addends */
  - HASH           5  /* Symbol hash table */
  - DYNAMIC        6  /* Dynamic linking information */
  - NOBITS         8  /* Program space with no data (bss) */
  - NOTE           7  /* Notes */
  - INIT_ARRAY    14  /* Array of constructors */
  - FINI_ARRAY    15  /* Array of destructors */
  - PREINIT_ARRAY 16  /* Array of pre-constructors */
  - GROUP         17  /* Section group */
  - SYMTAB_SHNDX  18  /* Extended section indeces */
- Flags
  - W 进程执行的时候, section内的数据可写
  - X 节内包含可以执行的机器指令
  - A 进程执行的时候，section 需要占据内存
- link & info
  - DYNAMIC 时, link 为 字符表 section header index
  - HASH 时, link 为 被 hash 的 符号表 section header index
  - REL / RELA 时, link 为 相关符号表 section header index, info 为
  - SYMTAB / DYNSYM 时, link 为 字符表 section header index, info 系统相关, 可为 第一个 non-local 符号的索引, 符号表大致是有序的, local在前面, WEAK / GOLBAL 不需严格的顺序

## .dynamic section

- NEEDED       依赖库
- SONAME       当前动态库名称
- RUNRPATH     运行时搜索路径, 在 LD_LIBRARY_PATH 之后

## example

```plain
There are 28 section headers, starting at offset 0x1918:

Section Headers:
  [Nr] Name              Type             Address           Offset
       Size              EntSize          Flags  Link  Info  Align
  [ 0]                   NULL             0000000000000000  00000000
       0000000000000000  0000000000000000           0     0     0
  [ 1] .interp           PROGBITS         0000000000400200  00000200
       000000000000001c  0000000000000000   A       0     0     1
  [ 2] .note.ABI-tag     NOTE             000000000040021c  0000021c
       0000000000000020  0000000000000000   A       0     0     4
  [ 3] .hash             HASH             0000000000400240  00000240
       000000000000002c  0000000000000004   A       5     0     8
  [ 4] .gnu.hash         GNU_HASH         0000000000400270  00000270
       0000000000000024  0000000000000000   A       5     0     8
  [ 5] .dynsym           DYNSYM           0000000000400298  00000298
       0000000000000090  0000000000000018   A       6     1     8
  [ 6] .dynstr           STRTAB           0000000000400328  00000328
       000000000000007d  0000000000000000   A       0     0     1
  [ 7] .gnu.version      VERSYM           00000000004003a6  000003a6
       000000000000000c  0000000000000002   A       5     0     2
  [ 8] .gnu.version_r    VERNEED          00000000004003b8  000003b8
       0000000000000020  0000000000000000   A       6     1     8
  [ 9] .rela.dyn         RELA             00000000004003d8  000003d8
       0000000000000078  0000000000000018   A       5     0     8
  [10] .init             PROGBITS         0000000000400450  00000450
       0000000000000017  0000000000000000  AX       0     0     4
  [11] .plt              PROGBITS         0000000000400470  00000470
       0000000000000010  0000000000000010  AX       0     0     16
  [12] .plt.got          PROGBITS         0000000000400480  00000480
       0000000000000008  0000000000000008  AX       0     0     8
  [13] .text             PROGBITS         0000000000400490  00000490
       00000000000001b2  0000000000000000  AX       0     0     16
  [14] .fini             PROGBITS         0000000000400644  00000644
       0000000000000009  0000000000000000  AX       0     0     4
  [15] .rodata           PROGBITS         0000000000400650  00000650
       0000000000000004  0000000000000004  AM       0     0     4
  [16] .eh_frame         PROGBITS         0000000000400658  00000658
       0000000000000118  0000000000000000   A       0     0     8
  [17] .init_array       INIT_ARRAY       0000000000600e18  00000e18
       0000000000000008  0000000000000008  WA       0     0     8
  [18] .fini_array       FINI_ARRAY       0000000000600e20  00000e20
       0000000000000008  0000000000000008  WA       0     0     8
  [19] .dynamic          DYNAMIC          0000000000600e28  00000e28
       00000000000001b0  0000000000000010  WA       6     0     8
  [20] .got              PROGBITS         0000000000600fd8  00000fd8
       0000000000000028  0000000000000008  WA       0     0     8
  [21] .got.plt          PROGBITS         0000000000601000  00001000
       0000000000000018  0000000000000008  WA       0     0     8
  [22] .data             PROGBITS         0000000000601018  00001018
       0000000000000018  0000000000000000  WA       0     0     8
  [23] .bss              NOBITS           0000000000601030  00001030
       0000000000000008  0000000000000000  WA       0     0     1
  [24] .comment          PROGBITS         0000000000000000  00001030
       0000000000000029  0000000000000001  MS       0     0     1
  [25] .symtab           SYMTAB           0000000000000000  00001060
       00000000000005e8  0000000000000018          26    42     8
  [26] .strtab           STRTAB           0000000000000000  00001648
       00000000000001ed  0000000000000000           0     0     1
  [27] .shstrtab         STRTAB           0000000000000000  00001835
       00000000000000dc  0000000000000000           0     0     1
Key to Flags:
  W (write), A (alloc), X (execute), M (merge), S (strings), I (info),
  L (link order), O (extra OS processing required), G (group), T (TLS),
  C (compressed), x (unknown), o (OS specific), E (exclude),
  l (large), p (processor specific)
```

### example dynamic section

```plain
Dynamic section at offset 0xe28 contains 22 entries:
  Tag        Type                         Name/Value
 0x0000000000000001 (NEEDED)             Shared library: [libc.so.6]
 0x000000000000000c (INIT)               0x400450
 0x000000000000000d (FINI)               0x400644
 0x0000000000000019 (INIT_ARRAY)         0x600e18
 0x000000000000001b (INIT_ARRAYSZ)       8 (bytes)
 0x000000000000001a (FINI_ARRAY)         0x600e20
 0x000000000000001c (FINI_ARRAYSZ)       8 (bytes)
 0x0000000000000004 (HASH)               0x400240
 0x000000006ffffef5 (GNU_HASH)           0x400270
 0x0000000000000005 (STRTAB)             0x400328
 0x0000000000000006 (SYMTAB)             0x400298
 0x000000000000000a (STRSZ)              125 (bytes)
 0x000000000000000b (SYMENT)             24 (bytes)
 0x0000000000000015 (DEBUG)              0x0
 0x0000000000000003 (PLTGOT)             0x601000
 0x0000000000000007 (RELA)               0x4003d8
 0x0000000000000008 (RELASZ)             120 (bytes)
 0x0000000000000009 (RELAENT)            24 (bytes)
 0x000000006ffffffe (VERNEED)            0x4003b8
 0x000000006fffffff (VERNEEDNUM)         1
 0x000000006ffffff0 (VERSYM)             0x4003a6
 0x0000000000000000 (NULL)               0x0
```
