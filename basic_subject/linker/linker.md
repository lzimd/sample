# linker

## symbol resolution 符号解析

### 顺序 - 相关：undefined reference

- 实践：把.o文件放到前面，把库文件放到最后
- 顺序：从左到右，linker对每个静态库都只是扫描一遍，不过如果用 --start-group 和 --end-group 包围起来的库则是重复性的扫描，直到没有新的未定义的符号被创建

### 处理多重定义的符号名的规则：

- 不允许有多个同名的强符号出现
- 一个强符号和多个弱符号同名，选择强符号
- 多个重名的弱符号，随便选择一个

对应到 GLOBAL / WEAK / LOCAL 符号：函数和已初始化的全局变量是强符号，未初始化的全局变量是弱符号

- 不允许 多个 GLOBAL symbol 出现
- 同名的 WEAK symbol 和 GLOBAL symbol 不会报错，选择 GLOBAL symbol
- 同名的 WEAK symbol 和 common symbol (位于common block中)，忽略 WEAK symbol
- 多个重名的 common symbol，随便选择一个

## relocation

- 重定位节和符号定义
- 重定位节中的符号引用

## symbol visibility 符号可见性

- static keyword: Visibility internal (translation unit only)
- gnu gcc:  `__attribute__ ((visibility ("hidden")));` or `-fvisibility=default|internal|hidden|protected`
- export file
  - gcc
    - static lib: --retain-symbols-file
    - shared lib: --version-script

## text sections / code

## data sections / data

Type: PROGBITS  1  Program data

Section:
.data       放 已经初始化的 全局变量 和 静态变量
.bss        放 未初始化的 静态变量 和 全局变量, 以及所有被初始化为 0 的静态变量 和 全局变量, 不占空间
.rodata     放 常量字符串之类的
.data1      放 已经初始化的 静态变量, compiler 相关
.rodata1

未初始化的全局变量, 放到 common block 或 .bss section

> <http://www.wowotech.net/basic_subject/compile-link-load.html>
>
> 虽然 static 的符号在源代码中没有初始化,
> 不过根据c标准的定义, 被 static 描述的对象需要在程序启动之前被初始化, 指针类型的被初始化成NULL, int被初始化成 0, 
> 这样的行为是和 .bss section 的行为类似的

.bss
> CSAPP
>
> 用 .bss 表示未初始化的数据是很普遍的.
> 起始于 IBM 704 汇编中 Block Storage Start, 沿用至今.

## common 符号 及 common blocks 机制

通常未初始化的全局变量就是该类型的符号, 全局静态变量受静态变量规则约束.
在 relocatable object file 中存在, linker 处理后移到 .bss 中

> <http://www.wowotech.net/basic_subject/compile-link-load.html>
>
> 传统的 unix 编译器是允许在多个编译单元 ( c文件 ) 中定义未初始化的全局变量的,
> 也就是说, 在两个c文件中都定义了名字一样的未初始化的全局变量是不会引起编译错误的,
> 编译器在编译的时候会把未初始化的全局变量放入到 common block 中而不是 .bss section.
>
> 假如放入到.bss section, 就意味着已经分配了该符号的地址, runtime 的时候会占用内存
> 那么linker 在合并 .bss section 的时候就会发现重复定义的符号了
>

gcc option: -fno-common / -fcommon
