add_library(debugger STATIC debuglib.cpp debuglib.h)
set_property(TARGET debugger PROPERTY POSITION_INDEPENDENT_CODE ON)

find_package(DWARF REQUIRED)
add_library(dwarfutil STATIC dwarfutil.cpp dwarfutil.h)
set_property(TARGET dwarfutil PROPERTY POSITION_INDEPENDENT_CODE ON)