# ELF section Symbol table

- `readelf -s prog.out`

## define

```c
typedef struct
{
  Elf64_Word    st_name;    /* Symbol name (string tbl index) */
  unsigned char st_info;    /* Symbol type and binding */
  unsigned char st_other;   /* Symbol visibility */
  Elf64_Section st_shndx;   /* Section index */
  Elf64_Addr    st_value;   /* Symbol value */
  Elf64_Xword   st_size;    /* Symbol size */
} Elf64_Sym;
```

## section

- .dynsym   SHT_DYNSYM  Dynamic linker symbol table (DT_SYMTAB)
- .symtab   SHT_SYMTAB  Full symbol table

### dynsym

是 .symtab 的子集, section Flags 为 A (allocable), 动态库加载时需要

### symtab

符号表, 可以被 strip, linker 和 debugger 需要

- FLAG BIND:
  - LOCAL    Local symbol
  - GLOBAL   Global symbol
  - WEAK     Weak symbol
- FLAG Type:
  - NOTYPE   unspecified
  - OBJECT   data object
  - FUNC     code object
  - SECTION  Symbol associated with a section, 表示一个 section, 主要用于重定位, 通常具有 STB_LOCAL 属性
  - FILE     Symbol's name is file name, 文件符号, 具有 STB_LOCAL 属性, st_shndx 的值为 SHN_ABS
  - COMMON   common data object
  - TLS      thread-local data object
- FLAG Vis(visibility)
  - DEFAULT
  - INTERNAL   Processor specific hidden class
  - HIDDEN     Sym unavailable in other modules
  - PROTECTED  Not preemptible, not exported
- FLAG Ndx:
  - UND     Undefined section
  - ABS     Associated symbol is absolute
  - COMMON  Associated symbol is common (common block)
  - XINDEX  number, Index is in extra table

### 结构体

对于不同类型的 object file, st_value 的含义略有不同：

- 对于 relocatable (.o) 文件, 如果相应的 st_shndx 的值为 SHN_COMMON, 则 st_value 保存的是对齐字节数
- 对于 relocatable (.o) 文件, 如果该符号已定义, 则 st_value 保存的是该符号在其所定义的 section (由 st_shndx 指定) 中的偏移量
- 对于 execuatable 和 shared object (.so) 文件, st_value 保存的是一个虚拟地址

## exmaple

```plain
Symbol table '.dynsym' contains 6 entries:
   Num:    Value          Size Type    Bind   Vis      Ndx Name
     0: 0000000000000000     0 NOTYPE  LOCAL  DEFAULT  UND
     1: 0000000000000000     0 NOTYPE  WEAK   DEFAULT  UND _ITM_deregisterTMCloneTab
     2: 0000000000000000     0 FUNC    GLOBAL DEFAULT  UND __libc_start_main@GLIBC_2.2.5 (2)
     3: 0000000000000000     0 NOTYPE  WEAK   DEFAULT  UND __gmon_start__
     4: 0000000000000000     0 NOTYPE  WEAK   DEFAULT  UND _ITM_registerTMCloneTable
     5: 0000000000000000     0 FUNC    WEAK   DEFAULT  UND __cxa_finalize@GLIBC_2.2.5 (2)

Symbol table '.symtab' contains 63 entries:
   Num:    Value          Size Type    Bind   Vis      Ndx Name
     0: 0000000000000000     0 NOTYPE  LOCAL  DEFAULT  UND
     1: 0000000000400200     0 SECTION LOCAL  DEFAULT    1
     2: 000000000040021c     0 SECTION LOCAL  DEFAULT    2
     3: 0000000000400240     0 SECTION LOCAL  DEFAULT    3
     4: 0000000000400270     0 SECTION LOCAL  DEFAULT    4
     5: 0000000000400298     0 SECTION LOCAL  DEFAULT    5
     6: 0000000000400328     0 SECTION LOCAL  DEFAULT    6
     7: 00000000004003a6     0 SECTION LOCAL  DEFAULT    7
     8: 00000000004003b8     0 SECTION LOCAL  DEFAULT    8
     9: 00000000004003d8     0 SECTION LOCAL  DEFAULT    9
    10: 0000000000400450     0 SECTION LOCAL  DEFAULT   10
    11: 0000000000400470     0 SECTION LOCAL  DEFAULT   11
    12: 0000000000400480     0 SECTION LOCAL  DEFAULT   12
    13: 0000000000400490     0 SECTION LOCAL  DEFAULT   13
    14: 0000000000400644     0 SECTION LOCAL  DEFAULT   14
    15: 0000000000400650     0 SECTION LOCAL  DEFAULT   15
    16: 0000000000400658     0 SECTION LOCAL  DEFAULT   16
    17: 0000000000600e18     0 SECTION LOCAL  DEFAULT   17
    18: 0000000000600e20     0 SECTION LOCAL  DEFAULT   18
    19: 0000000000600e28     0 SECTION LOCAL  DEFAULT   19
    20: 0000000000600fd8     0 SECTION LOCAL  DEFAULT   20
    21: 0000000000601000     0 SECTION LOCAL  DEFAULT   21
    22: 0000000000601018     0 SECTION LOCAL  DEFAULT   22
    23: 0000000000601030     0 SECTION LOCAL  DEFAULT   23
    24: 0000000000000000     0 SECTION LOCAL  DEFAULT   24
    25: 0000000000000000     0 FILE    LOCAL  DEFAULT  ABS crtstuff.c
    26: 00000000004004c0     0 FUNC    LOCAL  DEFAULT   13 deregister_tm_clones
    27: 0000000000400500     0 FUNC    LOCAL  DEFAULT   13 register_tm_clones
    28: 0000000000400550     0 FUNC    LOCAL  DEFAULT   13 __do_global_dtors_aux
    29: 0000000000601030     1 OBJECT  LOCAL  DEFAULT   23 completed.7697
    30: 0000000000600e20     0 OBJECT  LOCAL  DEFAULT   18 __do_global_dtors_aux_fin
    31: 0000000000400590     0 FUNC    LOCAL  DEFAULT   13 frame_dummy
    32: 0000000000600e18     0 OBJECT  LOCAL  DEFAULT   17 __frame_dummy_init_array_
    33: 0000000000000000     0 FILE    LOCAL  DEFAULT  ABS main.i
    34: 0000000000000000     0 FILE    LOCAL  DEFAULT  ABS sum.c
    35: 0000000000000000     0 FILE    LOCAL  DEFAULT  ABS crtstuff.c
    36: 000000000040076c     0 OBJECT  LOCAL  DEFAULT   16 __FRAME_END__
    37: 0000000000000000     0 FILE    LOCAL  DEFAULT  ABS
    38: 0000000000600e20     0 NOTYPE  LOCAL  DEFAULT   17 __init_array_end
    39: 0000000000600e28     0 OBJECT  LOCAL  DEFAULT   19 _DYNAMIC
    40: 0000000000600e18     0 NOTYPE  LOCAL  DEFAULT   17 __init_array_start
    41: 0000000000601000     0 OBJECT  LOCAL  DEFAULT   21 _GLOBAL_OFFSET_TABLE_
    42: 0000000000400640     2 FUNC    GLOBAL DEFAULT   13 __libc_csu_fini
    43: 0000000000000000     0 NOTYPE  WEAK   DEFAULT  UND _ITM_deregisterTMCloneTab
    44: 0000000000601018     0 NOTYPE  WEAK   DEFAULT   22 data_start
    45: 0000000000601028     8 OBJECT  GLOBAL DEFAULT   22 array
    46: 0000000000601030     0 NOTYPE  GLOBAL DEFAULT   22 _edata
    47: 0000000000400644     0 FUNC    GLOBAL DEFAULT   14 _fini
    48: 0000000000000000     0 FUNC    GLOBAL DEFAULT  UND __libc_start_main@@GLIBC_
    49: 0000000000601018     0 NOTYPE  GLOBAL DEFAULT   22 __data_start
    50: 0000000000000000     0 NOTYPE  WEAK   DEFAULT  UND __gmon_start__
    51: 0000000000601020     0 OBJECT  GLOBAL HIDDEN    22 __dso_handle
    52: 00000000004005b4    27 FUNC    GLOBAL DEFAULT   13 sum
    53: 0000000000400650     4 OBJECT  GLOBAL DEFAULT   15 _IO_stdin_used
    54: 00000000004005d0   101 FUNC    GLOBAL DEFAULT   13 __libc_csu_init
    55: 0000000000601038     0 NOTYPE  GLOBAL DEFAULT   23 _end
    56: 0000000000400490    43 FUNC    GLOBAL DEFAULT   13 _start
    57: 0000000000601030     0 NOTYPE  GLOBAL DEFAULT   23 __bss_start
    58: 000000000040059a    26 FUNC    GLOBAL DEFAULT   13 main
    59: 0000000000601030     0 OBJECT  GLOBAL HIDDEN    22 __TMC_END__
    60: 0000000000000000     0 NOTYPE  WEAK   DEFAULT  UND _ITM_registerTMCloneTable
    61: 0000000000000000     0 FUNC    WEAK   DEFAULT  UND __cxa_finalize@@GLIBC_2.2
    62: 0000000000400450     0 FUNC    GLOBAL DEFAULT   10 _init
```
