/*
    colours.h - ASCII colour codes
    Copyright (C) 2022  Thomas Cort

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.

*/

#ifndef TC_COLOURS_H
#define TC_COLOURS_H

#define COLOUR_RESET   "\x1b[0m"

#define COLOUR_BLACK   "\x1b[30m"
#define COLOUR_RED     "\x1b[31m"
#define COLOUR_GREEN   "\x1b[32m"
#define COLOUR_YELLOW  "\x1b[33m"
#define COLOUR_BLUE    "\x1b[34m"
#define COLOUR_MAGENTA "\x1b[35m"
#define COLOUR_CYAN    "\x1b[36m"
#define COLOUR_WHITE   "\x1b[37m"

#define COLOUR_BRIGHT_BLACK   "\x1b[30;1m"
#define COLOUR_BRIGHT_RED     "\x1b[31;1m"
#define COLOUR_BRIGHT_GREEN   "\x1b[32;1m"
#define COLOUR_BRIGHT_YELLOW  "\x1b[33;1m"
#define COLOUR_BRIGHT_BLUE    "\x1b[34;1m"
#define COLOUR_BRIGHT_MAGENTA "\x1b[35;1m"
#define COLOUR_BRIGHT_CYAN    "\x1b[36;1m"
#define COLOUR_BRIGHT_WHITE   "\x1b[37;1m"

#endif
