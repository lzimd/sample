#!/bin/bash

#
# http://www.alexonlinux.com/how-debugger-works
#

#
# https://eli.thegreenplace.net/2011/01/23/how-debuggers-work-part-1
#
# ./build/bin/simple_tracer ./build/bin/traced_helloworld
# ./build/bin/simple_tracer ./build/bin/traced_printer
#
# https://eli.thegreenplace.net/2011/01/27/how-debuggers-work-part-2-breakpoints
#  
# ./build/bin/tracer ./build/bin/traced_printer2 0x04000cb
#
# ./checksec.sh --file ./build/bin/traced_breakpoint
# addr_do_stuff=`objdump -d ./build/bin/traced_breakpoint | grep do_stuff | cut -d\  -f1`
# ./build/bin/tracer_breakpoint ./build/bin/traced_breakpoint 0x$addr_do_stuff
# if [ "$?" -ne 0 ]; then
#     echo -e "\033[0;31m Trap breakpoint failed. Maybe PIE enabled.\e[0m \n"
# fi
#
# https://eli.thegreenplace.net/2011/02/07/how-debuggers-work-part-3-debugging-information
#
# ./checksec.sh --file ./build/bin/traced_prog2
# objdump -h ./build/bin/traced_prog2
# file ./build/bin/traced_prog2 | grep debug_info
# if [ "$?" -ne 0 ]; then
    # echo -e "\033[0;31m No debugging symbols found. \e[0m"
# fi
# ./build/bin/dwarf_get_func_addr ./build/bin/traced_prog2

#
# Playing with ptrace, Part I  https://www.linuxjournal.com/article/6100
# linux ptrace I  https://www.cnblogs.com/mmmmar/p/6040325.html
#
# ./build/bin/simple_tracer_hello
# ./build/bin/simple_tracer_syscall
# ./build/bin/simple_tracer_regs
# ./build/bin/simple_tracer_inject
# ./build/bin/simple_tracer_step ./build/bin/traced_printer2
#
# Playing with ptrace, Part II https://www.linuxjournal.com/article/6210
# linux ptrace II  https://www.cnblogs.com/mmmmar/p/6048711.html
#
# ./build/bin/simple_tracer_attach
# ./build/bin/tracer_inject ./build/bin/traced_loop
