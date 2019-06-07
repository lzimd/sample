# static keyword

在 C 语言中, static是一个存储修饰符(storage-class specifier)

|Storage class | Lifetime |Visibility|
|:-|:-:|:-|
|extern     | program execution | external (whole program) |
|static     | program execution | internal (translation unit only) |
|auto, register |function execution | (none) |

全局变量, 默认是 extern 存储

局部变量, 默认是 auto 存储

## static 用法

- 全局静态变量, 和全局变量存储地方一致, 但只能在定义它的源文件里访问
- 局部静态变量, 和局部变量存储地方不一致, 放在 .data / .bss 段, 程序的整个生命周期内有效, 只能被作用域内被访问
  - 多线程需要注意
- 静态函数

## 目标文件中

符号表 BIND 属性为 LOCAL

已经初始化的 static 变量放到 .data section

未初始化的 static 变量放到 .bss section

初始化为 0 的 static 变量放到 .bss section

```plain
0000000000201014 l     O .data  0000000000000004              gw_s_i
0000000000201024 l     O .bss   0000000000000004              gw_s_i_zero
0000000000201028 l     O .bss   0000000000000004              gw_s_u

0000000000201018 l     O .data  0000000000000004              lc_s_i.2255
0000000000201030 l     O .bss   0000000000000004              lc_s_i_zero.2256
000000000020102c l     O .bss   0000000000000004              lc_s_u.2257

0000000000201010 g     O .data  0000000000000004              gw_i
0000000000201020 g     O .bss   0000000000000004              gw_i_zero
0000000000201034 g     O .bss   0000000000000004              gw_u
```
