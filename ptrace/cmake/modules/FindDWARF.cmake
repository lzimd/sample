# - Find Dwarf
# Find the dwarf.h header from elf utils
# Only find libdwarf
# 
# Also 
# https://github.com/acmel/dwarves/blob/master/cmake/modules/FindDWARF.cmake
#
# libdwarf  depend on libelf 
#   /usr/include/libdwarf/dwarf.h
#   /usr/include/libdwarf/libdwarf.h
#   libdwarf
#
# libdw     depend on libelf 
#   /usr/include/dwarf.h
#   /usr/include/elfutils/known-dwarf.h
#   /usr/include/elfutils/libdw.h
#   /usr/include/elfutils/libdwelf.h
#   /usr/include/elfutils/libdwfl.h
#   libebl
#   libdw
#
#  DWARF_INCLUDE_DIR - where to find dwarf.h, etc.
#  DWARF_LIBRARIES   - List of libraries when using elf utils.
#  DWARF_FOUND       - True if fdo found.

INCLUDE(CheckLibraryExists)

if (DWARF_INCLUDE_DIR)
	set(DWARF_FIND_QUIETLY TRUE)
endif (DWARF_INCLUDE_DIR)

find_path(DWARF_INCLUDE_DIR libdwarf/libdwarf.h
	/usr/include
	/usr/include/libdwarf
)

if(DWARF_INCLUDE_DIR)
	set(DWARF_FOUND TRUE)
else()
	set(DWARF_FOUND FALSE)
endif()

if (NOT DWARF_FOUND)
	if (DWARF_FIND_REQUIRED)
		find_file(DEBIAN lsb-release /etc)
		if (DEBIAN)
			message("Please install package(dwarfutils): libdwarf1 libdwarf-dev")
		endif(DEBIAN)
		message(FATAL_ERROR "Could NOT find some ELF and DWARF libraries.")
	endif()
endif()

mark_as_advanced(DWARF_INCLUDE_DIR)
include_directories(${DWARF_INCLUDE_DIR})
