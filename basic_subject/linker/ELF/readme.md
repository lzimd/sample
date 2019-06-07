# ELF

- ELF header
- File data
  - Program segment header   Elf64_Phdr
  - Section Headers          Elf64_Shdr
  - Data

## 例子

以 helloworld/prog.out 为例,

1 )

- .text section / .data section / .rodata section / .bss section
- text segment / data segemnt / heap / free memory / stack

2 ) segment

data segment 起始位置

使用 -static 编译, 增加点 text segment 长度, ld 默认启用 -z relro, `gcc -static main.c && readelf -e -Wl a.out`

text segment, loader 映射 exe file 到 进程地址空间, 从 0x400000 开始, 长度 0x0b58fe, align 0x200000

data segment, loader 映射 exe file 到 进程地址空间, 从 0x6B6120 开始, 长度 0x005198, align 0x200000
首个 section (.tdata) 偏移为 b6120, 起始地址 vaddr = 0x6B6120

不过为什么 第一个 loadable segment 长度在 0x0b58fe, 第二个 loadable segment 要从 0x0b6120 开始呢 ?
因为要满足对齐要求, 方便有效率地加载到内存中.

loadable segement 对齐 是和 page size 相关的.
在 x86_64 MAXPAGESIZE 为 0x200000 ( 512k ) 时, 0x0b6120 没有 512k, 但也要满足其他页大小对齐, 所以 linker script 中还有个 COMMONPAGESIZE.
但 x86_64 COMMONPAGESIZE 为 0x1000 ( 1k ), 0x0b6120 也不满足 1k 对齐, 为什么 ?
这是因为 用的 ld 默认开启了 RELRO, 如果使用 `ld -z norelro`, 就可以看到 1k 对齐了.
可 开启了 RELRO, 也需要满足对齐, 所以真相是, GNU_RELRO segment 结束地址满足对齐要求, 使得紧接着的读写数据区满足对齐.
详细来说, .got.plt 起始位置 = `DATA_SEGMENT_RELRO_END (SIZEOF (.got.plt) >= 24 ? 24 : 0, .)` = 0x06B9000‬, 满足对齐.

```plain
0x6B6120 开始, 截至 0x6B9000‬, RELRO, 包括 .tdata .tbss .init_array .fint_array .data.rel.ro .got
0x6B9000‬ 满足 DATA_SEGMENT_ALIGN (CONSTANT (MAXPAGESIZE), CONSTANT (COMMONPAGESIZE))
0x6B9000‬ 开始, 截至 0x6BB2D8, 包括 .got.plt .data
0x6BB2D8 开始, 截至 0x6BCA00, 包括 .bss, 此时 不再映射 exe file, 但 mem 还是要有空间的
```

所以, 如果有 PT_GNU_RELRO, 要保证 GNU_RELRO segment 结束地址满足对齐要求; 第二个 load segment (=RW) 和 TLS segment, GNU_RELRO segment 会有重叠部分

> binutils/docs/ld/Builtin-Functions.html
>
> DATA_SEGMENT_RELRO_END
> This defines the end of the PT_GNU_RELRO segment when ‘-z relro’ option is used.
> When ‘-z relro’ option is not present, DATA_SEGMENT_RELRO_END does nothing,
> otherwise DATA_SEGMENT_ALIGN is padded so that exp + offset is aligned to the commonpagesize argument given to DATA_SEGMENT_ALIGN.
> If present in the linker script, it must be placed between DATA_SEGMENT_ALIGN and DATA_SEGMENT_END.
> Evaluates to the second argument plus any padding needed at the end of the PT_GNU_RELRO segment due to section alignment.

3 ) section
0x400000 + SIZEOF_HEADERS
    = size(ELF header) + size(Program segment header)
    = 64 + 56 * 8 = 0x200

所以 第一个 Section ( .initer ) 从 0x00400200 开始

4 ) header

ELF header 中的 ident -> data, LSB( 1 小端) / MSB ( 2 大端 ), 对 ELF header 之后的数据起作用

5 ) RELRO 只读段

GNU_RELRO: .init_array .fini_array .dynamic .got

## ld linker script

```ld
/* Read-only sections, merged into text segment: */
PROVIDE (__executable_start = SEGMENT_START("text-segment", 0x400000));
. = SEGMENT_START("text-segment", 0x400000) + SIZEOF_HEADERS;

...

/* Adjust the address for the data segment.  We want to adjust up to
    the same address within the page on the next page up.  */
. = DATA_SEGMENT_ALIGN (CONSTANT (MAXPAGESIZE), CONSTANT (COMMONPAGESIZE));

...

.dynamic        : { *(.dynamic) }
.got            : { *(.got) *(.igot) }
. = DATA_SEGMENT_RELRO_END (SIZEOF (.got.plt) >= 24 ? 24 : 0, .);
.got.plt        : { *(.got.plt)  *(.igot.plt) }
.data

...

. = DATA_SEGMENT_END (.)
/* Stabs debugging sections.  */
/* DWARF debug sections. */
```

## 更多资料

- <https://www.gabriel.urdhr.fr/2015/09/28/elf-file-format/>
- <https://linux-audit.com/elf-binaries-on-linux-understanding-and-analysis/>
- <http://refspecs.linuxfoundation.org/elf/x86_64-abi-0.99.pdf>
- <https://www.cnblogs.com/ilocker/tag/ELF/>
- <https://docs.oracle.com/cd/E19683-01/816-1386/chapter6-94076/index.html>
- <https://www.cnblogs.com/jiqingwu/p/elf_explore_1.html>
- <https://www.cnblogs.com/jiqingwu/p/elf_explore_2.html>
- <https://www.cnblogs.com/jiqingwu/p/elf_explore_3.html>
