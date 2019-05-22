> 在X86_64体系中，系统调用号保存在rax，调用参数依次保存在rdi,rsi,rdx,rcx,r8,r9
> 在x86体系中，   系统调用号保存在eax，调用参数依次保存在ebx,ecx,edx,esi,edi,ebp

syscall
%rax  Name      %rdi                          %rsi          %rdx    %rcx    %r8
%eax  Name      %ebx                          %ecx          %edx    %esx    %edi
1     sys_exit  int error_code                -             -       -       -
2     sys_fork  struct pt_regs                -             -       -       -
3     sys_read  unsigned int fd               char *        size_t  -       -
4     sys_write unsigned int fd               const char *  size_t  -       -
5     sys_open  const char __user *filename   int           int     -       -
6     sys_close unsigned int fd    -          -       -     -       -       -


## user_regs_struct
- /usr/include/x86_64-linux-gnu/sys/user.h

  - __x86_64__

    ```c
    struct user_regs_struct
    {
      unsigned long r15;
      unsigned long r14;
      unsigned long r13;
      unsigned long r12;
      unsigned long rbp;
      unsigned long rbx;
      unsigned long r11;
      unsigned long r10;
      unsigned long r9;
      unsigned long r8;
      unsigned long rax;
      unsigned long rcx;
      unsigned long rdx;
      unsigned long rsi;
      unsigned long rdi;
      unsigned long orig_rax;
      unsigned long rip;
      unsigned long cs;
      unsigned long eflags;
      unsigned long rsp;
      unsigned long ss;
      unsigned long fs_base;
      unsigned long gs_base;
      unsigned long ds;
      unsigned long es;
      unsigned long fs;
      unsigned long gs;
    };
    ```

  - i386
  
    ```c
    struct user_regs_struct
    {
      long int ebx;
      long int ecx;
      long int edx;
      long int esi;
      long int edi;
      long int ebp;
      long int eax;
      long int xds;
      long int xes;
      long int xfs;
      long int xgs;
      long int orig_eax;
      long int eip;
      long int xcs;
      long int eflags;
      long int esp;
      long int xss;
    };
    ```