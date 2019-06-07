# Linking

> 蜗窝科技 - 计算机科学基础知识

- Relocatable Object File <http://www.wowotech.net/basic_subject/compile-link-load.html>
- 静态库和静态链接 <http://www.wowotech.net/basic_subject/static-link.html>
- 动态库和位置无关代码 <http://www.wowotech.net/basic_subject/pic.html>
- 动态链接 <http://www.wowotech.net/basic_subject/dynamic-link.html>

> CSAPP

- 第七章 链接

## 目标文件

- PE, Portable Executable, Windows 使用
- ELF, Executable and Linkable Format, Linux 使用

文件类型有 Relocatable file, Executable file, Shared object file 等

- Relocatable file

.o 文件, 编译结果, 包含适合于与其他目标文件链接来创建可执行文件或者共享目标文件的代码和数据, 源码经过预处理 ( Preprocessing ), 编译 ( Compilation ), 汇编 ( Assembly ) 环节得到.

## 静态链接

以一组 .o 文件, 生成一个完全链接的, 可以加载和运行的可执行目标文件.

任务:

- symbol resolution
- relocation
  - 重定位节和符号定义
  - 重定位节中的符号引用

## 动态链接

共享库是一个目标模块, 在运行或加载时, 可以加载到任意的内存地址, 并和一个在内存中的程序链接起来.

不可以加载到任意的内存地址, 会有一些麻烦的问题, 空间利用, 分配地址, 系统相关问题等等.
可以加载到任意的内存地址, 通过 PIC 来实现.

PIC 数据引用 使用 GOT , PIC 函数调用 使用 GOT PLT.

任务:

- 重定位 .so 的 text 和 data 到某个内存段
- 重定位 程序中对 .so 中定义的符号的引用

## 目标文件中的对齐

对于 segment, 第一个 section 起始地址要满足 vaddr mod align = off mod align, 这样目标文件能有效率地传输到内存中.

## 术语

- ROP Return-oriented programming
- gadgets
  - [ROPgadget](https://github.com/JonathanSalwan/ROPgadget)

## 更多

- CSAPP
- ELF file format <https://www.gabriel.urdhr.fr/2015/09/28/elf-file-format/>
- Linux Foundation Referenced Specifications <http://refspecs.linuxfoundation.org/>
- Program Library HOWTO <http://tldp.org/HOWTO/Program-Library-HOWTO/index.html>
- Linker and Libraries Guide > Chapter 7 Object File Format > Sections <https://docs.oracle.com/cd/E19683-01/816-1386/chapter6-94076/index.html>
- symbol-visibility part 1 <https://www.ibm.com/developerworks/aix/library/au-aix-symbol-visibility/index.html>
- symbol-visibility part 2 <https://www.ibm.com/developerworks/aix/library/au-aix-symbol-visibility-part2/index.html>
