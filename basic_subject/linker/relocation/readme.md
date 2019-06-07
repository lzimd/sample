# relocation

## static linking

- Relocating sections and symbol definitions. 重定位 节和 符号定义.
  - 在这一步完成之后, 每条指令和全局变量都有唯一的运行时内存地址
- Relocating symbol references within sections. 重定位节中的符号引用
  - 在这一步完成之后, 节中符号引用不再是 0x000000, 指向正确的运行时地址.

### Relocating sections 重定位 节

linker 将 所有相同类型的 section 合并成 同一类型的聚合 section.

通过 relocating_sections 中的实验可知,
main.o .text 长度 0x20, arr 在 0x02 偏移处, sum-sub.o .text 长度 0x1c, sum 在 0x00 偏移处.
合并后的 sum.elf 中 .text 长度 0x3c, arr 在 0x02 偏移处, sum 在 0x20 偏移处.
两个 .o 的 .data 和 .bss 长度都为 0 , 所以在 sum.elf 也就没 .data 和 .bss.

## Relocating symbol definitions 重定位 符号定义

linker 在 合并 section 时, 会重定位 符号定义.

通过 relocating_sections 中的实验可知, 使用不同的 `-Ttext=`, arr 的偏移是不一样的.

### Relocating symbol references 重定位 符号引用

linker 修改 .text 和 .data 中的符号引用, 不再是 .o 文件中的 0x000000, 指向正确的运行时地址.

在 relocating_sections 中的实验中, 是有一处符号引用的, 查看 main.o, 可以看到 0x1b 处为 0x00000000, .rela.text 有相关的重定位条目.

R_X86_64_PC32 是 相对地址的引用, linker 会根据 `S + A - P` 的规则, 算出正确地址, 替换 0x1b 处的 0x00000000 为 0x00000001, 这样就可以 call  0x20 = 0x1f + 0x01 调用 sum 了.

其中, refaddr = addr ( .text section ) + r.offset = 0x1b ; *refaddr = addr ( sum symbol ) + r.addend - refaddr = 0x20 - 4 - 0x1b = 0x01

```plain
relocatable object file

Relocation section '.rela.text' at offset 0x118 contains 1 entry:
  Offset          Info           Type           Sym. Value    Sym. Name + Addend
00000000001b  000600000002 R_X86_64_PC32     0000000000000000 sum - 4

000000000000000e <_start>:
   e:   48 8d 3d ed ff ff ff    lea    -0x13(%rip),%rdi        # 2 <arr>
  15:   b8 00 00 00 00          mov    $0x0,%eax
  1a:   e8 00 00 00 00          callq  1f <_start+0x11>
                        1b: R_X86_64_PC32       sum-0x4
  1f:   c3                      retq


executable file

000000000000000e <_start>:
   e:   48 8d 3d ed ff ff ff    lea    -0x13(%rip),%rdi        # 2 <arr>
  15:   b8 00 00 00 00          mov    $0x0,%eax
  1a:   e8 01 00 00 00          callq  20 <sum>
  1f:   c3                      retq  

0000000000000020 <sum>:
```

## relocation entries 重定位条目

```c
typedef struct
{
  Elf64_Addr    r_offset;   /* Address */
  Elf64_Xword   r_info;     /* Relocation type and symbol index */
  Elf64_Sxword  r_addend;   /* Addend */
} Elf64_Rela;
```

## R_X86_64_COPY and R_X86_64_GLOB_DAT

> <https://stackoverflow.com/questions/56430874/memory-location-of-variables-defined-in-a-shared-library>

- R_X86_64_COPY is a relocation type for initialized **external** variables defined in a different ELF object.
- R_X86_64_GLOB_DAT is the corresponding a relocation type for globally-visible objects whose initial value is stored in the **current** ELF object.

如果使用 `gcc -fPIC -c main.c -o main.o`, extern 变量在符号表中为 `UND`, 重定向类型为 `R_X86_64_PC32`, 在使用动态库时会变成 `R_X86_64_COPY`

如果使用 `gcc -fPIC -c main.c -o main.o`, extern 变量会变成 `R_X86_64_REX_GOTPCRELX`, 以至于在  executable 文件中变成 `R_X86_64_GLOB_DAT`

## R_X86_64_GLOB_DAT and R_X86_64_JUMP_SLOT

GOT and PLT 会用到

## .rela.dyn 前面的几个  R_X86_64_RELATIVE

```plain
  Offset          Info           Type           Sym. Value    Sym. Name + Addend
000000200df8  000000000008 R_X86_64_RELATIVE                    6e0
000000200e00  000000000008 R_X86_64_RELATIVE                    6a0
000000201020  000000000008 R_X86_64_RELATIVE                    201020
000000201030  000000000008 R_X86_64_RELATIVE                    755
```

大致为 frame_dummy, __do_global_dtors_aux, .data, .rodata 之类的地址, 这些都要运行加载时重定位.
无名符号是因为  frame_dummy, __do_global_dtors_aux 这些是 LOCAL, 符号名字只在 .strtab 出现, 而 GLOBAL 符号和 WARK 符号名字在.strtab 和 .dynstr 都会有.

## ELF section

```plain
.rela.dyn
.rela.plt

.rela.text     .rela.data      .rela.bss
.rela.init     .rela.fini      .rela.rodata     .rela.tdata     .rela.tbss
.rela.ctors    .rela.dtors     .rela.ldata      .rela.lbss      .rela.lrodata
.rela.got
.rela.ifunc

.rela.eh_frame
```

- .rel.text 代码的重定位条目
- .rel.data 已初始化变量的重定位条目
  - 例子: `int var = &pp;` pp 为引入符号, 需要在 .rel.data 有重定位条目

## more

- Embedded Programming with the GNU Toolchain - Linker <http://www.bravegnu.org/gnu-eprog/linker.html>
