# CMakeLists.txt
# Copyright 2022  Thomas Cort
#
# Copying and distribution of this file, with or without modification, are
# permitted in any medium without royalty, provided the copyright notice and
# this notice are preserved. This file is offered as-is, without any warranty.

cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(tcutils VERSION 2022 LANGUAGES C)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})
list(APPEND CURSES_PROGS clear snek)

list(APPEND PROGS arch basename cat cmp coinflip copy d6 date dc dirname)
list(APPEND PROGS dos2unix echo expand extract factor false fgrep grep head)
list(APPEND PROGS hexdump hostid hostname kill leave line link litc ln logger)
list(APPEND PROGS logname lowercase ls luhn machine makewords md2 mismatch moo)
list(APPEND PROGS mugwump nl printenv pwd rev rmdir scrub shell sleep spell)
list(APPEND PROGS stat sum sync tmpl true tty uname unexpand unix2dos uuidgen)
list(APPEND PROGS wc whoami xkcdpass yes)


foreach(PROG IN LISTS PROGS CURSES_PROGS)
    add_executable(${PROG} src/${PROG}.c)

    list(FIND CURSES_PROGS ${PROG} CURSES_NEEDED)
    if (NOT (${CURSES_NEEDED} EQUAL -1))
        target_link_libraries(${PROG} ${CURSES_LIBRARY})
    endif()

    install(TARGETS ${PROG} DESTINATION bin)
endforeach()

