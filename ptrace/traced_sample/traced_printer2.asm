section .text
; The _start symbol must be declared for the linker (ld)
global _start

_start:
    ; sys_write(stdout, message, length)
    mov     rax, 1      ; sys_write
    mov     rdi, 1      ; stdout
    mov     rsi, msg1   ; message
    mov     rdx, len1   ; length
    syscall
    ; sys_write(stdout, message, length)
    mov     rax, 1      ; sys_write
    mov     rdi, 1      ; stdout
    mov     rsi, msg2   ; message
    mov     rdx, len2   ; length
    syscall
    ; sys_exit(return_code)
    mov     rax, 60     ; sys_exit
    mov     rdi, 0      ; return_code
    syscall

section  .data
	msg1    db      'Hello,', 0xa
	len1    equ     $ - msg1
	msg2    db      'world!', 0xa
	len2    equ     $ - msg2
