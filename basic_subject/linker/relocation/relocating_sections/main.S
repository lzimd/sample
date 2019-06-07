.text

.globl    _start

    jmp _start

arr:
    .long    1
    .long    2
    .long    3

    .align

_start:
    leaq    arr(%rip), %rdi
    movl    $0, %eax
    call    sum
    ret
