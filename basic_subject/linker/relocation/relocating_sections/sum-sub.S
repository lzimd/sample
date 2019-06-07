    .text
    .globl    sum

sum:
.LFB0:
    movl    $0, %edx
    movl    $0, %eax
    jmp    .L2
.L3:
    movslq  %edx, %rcx
    addl    (%rdi,%rcx,4), %eax
    addl    $1, %edx
.L2:
    cmpl    $2, %edx
    jle    .L3
    rep ret
