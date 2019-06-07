# PIC - Position Independent Code

位置无关代码

动态库可以被加载到任何地址并正确运行.

模块内的 数据引用 和 函数调用, 可使用 相关地址引用. 模块外的 数据引用 和 函数调用, 要用一些技巧.

## 数据引用

GOT, 全局偏移量表, Golbal Offset Table, 可读可写, runtime 修改为 重定向地址.

每个被引用的全局数据目标都有一个 GOT 条目, 每个条目有重定位记录.
每一个引用全局目标的目标模块得有自己的 GOT.

## 函数调用

一般 Lazy relocations, 但目前有些 toolchain 会 `-z now -z relro` 做 完全RELRO, 导致惰性解析禁用.

PLT, 过程链接表, Procedure Linkage Table, 当一个外部符号被调用时, PLT 去引用 GOT 中的其符号对应的绝对地址, 然后转入并执行.
GOT, 全局偏移量表, Golbal Offset Table, 第一次是空, 所以会跳回func@PLT+2, 使用 PLT[0] call dynamic linkerq 去解析符号, 并写回GOT, 第二次PLT再调用, 直接转去执行.

GOT [0] addr of .dynamic
GOT [1] addr of reloc, addr of link_map
GOT [2] addr of dynamic linker, addr of _dl_runtime_resolve

PLT [0] call dynamic linker

## example

shared library section

- .dynamic
- .dynsym
- .rela.dyn
- .dynstr

```plain
.dynsym
Num:    Value          Size Type    Bind   Vis      Ndx Name
5: 0000000000201020     4 OBJECT  GLOBAL DEFAULT   17 xxx
```

execute file section

```plain
.rela.dyn
Offset          Info           Type           Sym. Value        Sym. Name + Addend
000000201010  000800000005 R_X86_64_COPY     0000000000201010   xxx       + 0


.dynsym
Num:    Value          Size Type    Bind   Vis      Ndx Name
8: 0000000000201010     4 OBJECT  GLOBAL DEFAULT   24 xxx

.text
 74e:   8b 05 bc 08 20 00       mov    0x2008bc(%rip),%eax        # 201010 <xxx> = 754‬ + 0x2008bc
 754:   89 c6                   mov    %eax,%esi
```

## 更多资料

- <https://eli.thegreenplace.net/2011/11/03/position-independent-code-pic-in-shared-libraries/>
- <https://eli.thegreenplace.net/2011/11/11/position-independent-code-pic-in-shared-libraries-on-x64>
- <https://rickgray.me/2015/08/07/use-gdb-to-study-got-and-plt/>
