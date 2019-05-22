; The _start symbol must be declared for the linker (ld)
global _start

section .text
_start:
    ; sys_write(stdout, message, length)
    mov     rax, 1      ; sys_write
    mov     rdi, 1      ; stdout
    mov     rsi, msg   ; message
    mov     rdx, len   ; length
    syscall
    ; sys_exit(return_code)
    mov     rax, 60     ; sys_exit
    mov     rdi, 0      ; return_code
    syscall

section  .data
	msg    db      'Hello,world!', 0xa
	len    equ     $ - msg
