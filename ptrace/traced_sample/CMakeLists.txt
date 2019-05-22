# https://cmake.org/cmake/help/v3.14/policy/CMP0083.html
# CMake 3.13 and lower did not add these link flags ( PIE ) when POSITION_INDEPENDENT_CODE is set.
# 
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -no-pie")

add_executable(traced_helloworld traced_helloworld.c)
add_executable(traced_loop traced_loop.c)

add_executable(traced_breakpoint traced_breakpoint.c)
set(TARGET traced_breakpoint PROPERTY POSITION_INDEPENDENT_CODE OFF)

add_executable(traced_prog2 traced_prog2.c)

set(CMAKE_ASM_NASM_LINK_EXECUTABLE "<CMAKE_LINKER> <OBJECTS> -o <TARGET> <LINK_LIBRARIES>")
enable_language(ASM_NASM)
add_executable(traced_printer traced_printer.asm)
add_executable(traced_printer2 traced_printer2.asm)



