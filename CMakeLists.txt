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
list(APPEND PROGS dos2unix echo expand extract factor false fgrep fold grep)
list(APPEND PROGS head hexdump hostid hostname kill leave line link litc ln)
list(APPEND PROGS logger logname lowercase ls luhn machine makewords md2)
list(APPEND PROGS mismatch mktemp moo mugwump nl printenv pwd rev rmdir scrub)
list(APPEND PROGS seq shell sleep spell stat sum sync tac tee tmpl true tty)
list(APPEND PROGS uname unexpand unix2dos uuidgen wc whoami xkcdpass yes)


foreach(PROG IN LISTS PROGS CURSES_PROGS)
    add_executable(${PROG} src/${PROG}.c)

    list(FIND CURSES_PROGS ${PROG} CURSES_NEEDED)
    if (NOT (${CURSES_NEEDED} EQUAL -1))
        target_link_libraries(${PROG} ${CURSES_LIBRARY})
    endif()

    install(TARGETS ${PROG} DESTINATION bin)
endforeach()

