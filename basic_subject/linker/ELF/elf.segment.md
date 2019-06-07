# ELF segment

- header length: 56 bytes for 64-bit, 32 bytes for 32-bit
- `readelf -l ./prog.out`

.so 和 exe 一般有两个 LOAD 段

linker 在加载时, 会根据 LOAD 段的最大虚拟内存和最小虚拟内存, 来计算申请的内存大小

semgent flag, R/W/X, 内存访问权限还要取决于系统的 MMU (内存管理单元), 系统给予的访问权限可能比指定的权限( segment flag ) 要大

.bss 节的类型是 SHT_NOBITS，表示在文件中不占空间, .bss 在段的内存映像中是占用空间的, 通常位于段的末尾, 用于存放未初始化的全局变量

readelf 中的 `Section to Segment mapping` 通过 section 和 segment 的 offset 和 size 计算得到.

## define

```c
typedef struct
{
  Elf64_Word     p_type;         /* Segment type */
  Elf64_Word     p_flags;        /* Segment flags */
  Elf64_Off      p_offset;       /* Segment file offset */
  Elf64_Addr     p_vaddr;        /* Segment virtual address */
  Elf64_Addr     p_paddr;        /* Segment physical address */
  Elf64_Xword    p_filesz;       /* Segment size in file */
  Elf64_Xword    p_memsz;        /* Segment size in memory */
  Elf64_Xword    p_align;        /* Segment alignment */
} Elf64_Phdr;
```

## program header

### segment type

- LOAD          1           /* Loadable program segment */
- DYNAMIC       2           /* Dynamic linking information */
- INTERP        3           /* Program interpreter */
- NOTE          4           /* Auxiliary information */
- PHDR          6           /* Entry for header table itself */
- TLS           7           /* Thread-local storage segment */
- GNU_EH_FRAME  0x6474e550  /* GCC .eh_frame_hdr segment */
- GNU_STACK     0x6474e551  /* Indicates stack executability */
- GNU_RELRO     0x6474e552  /* Read-only after relocation */

### semgent flag

- PF_X  (1 << 0)    /* Segment is executable */
- PF_W  (1 << 1)    /* Segment is writable */
- PF_R  (1 << 2)    /* Segment is readable */

### GNU_EH_FRAME

This is a sorted queue used by the GNU C compiler (gcc).
It stores exception handlers. So when something goes wrong, it can use this area to deal correctly with it.

## example

```plain
Elf file type is EXEC (Executable file)
Entry point 0x400490
There are 8 program headers, starting at offset 64

Program Headers:
  Type           Offset             VirtAddr           PhysAddr
                 FileSiz            MemSiz              Flags  Align
  PHDR           0x0000000000000040 0x0000000000400040 0x0000000000400040
                 0x00000000000001c0 0x00000000000001c0  R      0x8
  INTERP         0x0000000000000200 0x0000000000400200 0x0000000000400200
                 0x000000000000001c 0x000000000000001c  R      0x1
      [Requesting program interpreter: /lib64/ld-linux-x86-64.so.2]
  LOAD           0x0000000000000000 0x0000000000400000 0x0000000000400000
                 0x0000000000000770 0x0000000000000770  R E    0x200000
  LOAD           0x0000000000000e18 0x0000000000600e18 0x0000000000600e18
                 0x0000000000000218 0x0000000000000220  RW     0x200000
  DYNAMIC        0x0000000000000e28 0x0000000000600e28 0x0000000000600e28
                 0x00000000000001b0 0x00000000000001b0  RW     0x8
  NOTE           0x000000000000021c 0x000000000040021c 0x000000000040021c
                 0x0000000000000020 0x0000000000000020  R      0x4
  GNU_STACK      0x0000000000000000 0x0000000000000000 0x0000000000000000
                 0x0000000000000000 0x0000000000000000  RW     0x10
  GNU_RELRO      0x0000000000000e18 0x0000000000600e18 0x0000000000600e18
                 0x00000000000001e8 0x00000000000001e8  R      0x1

 Section to Segment mapping:
  Segment Sections...
   00
   01     .interp
   02     .interp .note.ABI-tag .hash .gnu.hash .dynsym .dynstr .gnu.version .gnu.version_r .rela.dyn .init .plt .plt.got .text .fini .rodata .eh_frame
   03     .init_array .fini_array .dynamic .got .got.plt .data .bss
   04     .dynamic
   05     .note.ABI-tag
   06
   07     .init_array .fini_array .dynamic .got
```
