include_directories(${PROJECT_SOURCE_DIR}/libdebugger)
link_directories(${PROJECT_BINARY_DIR}/lib)

add_executable(tracer tracer.cpp)
target_link_libraries(tracer debugger)

add_executable(tracer_breakpoint tracer_breakpoint.cpp)
target_link_libraries(tracer_breakpoint debugger)

add_executable(tracer_inject tracer_inject.cpp)
target_link_libraries(tracer_inject debugger)

find_package(DWARF REQUIRED)
add_executable(dwarf_get_func_addr dwarf_get_func_addr.cpp)
target_link_libraries(dwarf_get_func_addr dwarf elf dwarfutil)
