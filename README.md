# tcutils

a collection of software tools, utilities, and games.

## Description

This software package includes dozens of standalone programs that are
intended to supplement, rather than replace, the system software. The
installation instructions provide directions for installing the software
alongside the system software.

There is also `libtc` which implements a portion of `libc` functionality.
It provides a portable, well defined, and consistent interface with unit tests.

## Requirements

* C compiler and standard build tools (make, sh, ...).
* [cmake](https://cmake.org/)
* curses library (e.g. [ncurses](https://invisible-island.net/ncurses/))
* [libcurl](https://curl.se/)

## Installation

Installing in your home directory:

```
cd bin
cmake -DCMAKE_INSTALL_PREFIX:PATH=${HOME} ..
make
make install
```

Add the following to `${HOME}/.profile`:

```
export PATH=${HOME}/bin:${PATH}
export PAGER=more

# disable internal commands (GNU bash)
enable -n echo
enable -n false
enable -n kill
enable -n pwd
enable -n true
```

## Programs Included

- arch -- print architecture type of this machine
- basename -- print the filename portion of a pathname
- cat -- concatenate and print files
- catfacts -- writes a random cat fact to standard output
- clear -- clear the terminal screen
- chef -- Swedish Chef translator
- cmp -- compare two files
- coinflip -- simulates a coinflip and prints the result
- copy -- copy characters from standard input to standard output
- crc32 -- output the 32-bit cyclic redundancy check value for an input
- d6 -- simulates a 6-sided dice roll and prints the reults
- date -- print the current date and time
- dc -- desk calculator
- dec2hex -- convert decimal to hexadecimal
- dirname -- print the directory portion of a pathname
- dos2unix -- replaces DOS line endings '\r\n' with UNIX line endings '\n'
- du -- show disk usage
- duck -- virtual rubber duck debugging tool
- echo -- prints it's command line arguments
- expand -- converts tabs to spaces
- extract -- extract values from the input and print them to the output
- factor -- prints the prime factors of a number
- false -- exits with a failure return code
- fgrep -- searches for and prints lines that exactly match a given string
- fold -- wraps long lines for fixed width viewing mediums
- forage -- find files, directories and more using regular expressions
- foreach -- executes a command for every line of standard input
- grep -- searches for and prints lines that match a given pattern
- head -- prints lines at the beginning of an input text
- hexdump -- prints the contents of a file in hexadecimal
- host -- DNS lookup utility
- hostid -- prints the current hostid of the system
- hostname -- prints the current hostname of the system
- kill -- kill a process
- leave -- kindly reminds the user to leave
- line -- read a line of input
- link -- creates a file link
- litc -- extract literate programs from markdown documents
- ln -- creates a file link
- loc -- counts lines of code
- logger -- add an entry in the system logs
- logname -- prints the current user's login name
- lolcat -- concatenate and print files with funky colours
- lowercase -- convert input text to lower case
- ls -- list the contents of a directory
- luhn -- luhn checker
- machine -- print architecture type of this machine
- makewords -- split the lines into words and print each on its own line
- md2 -- calculate a message-digest fingerprint for a file
- mismatch -- finds lines that are in input but not in FILE
- mkdir -- create directories
- mktemp -- create a temporary file or directory with a unique name
- moo -- guess the bulls
- more -- filter for paging through text, one screen at a time
- mugwump -- find all the mugwumps
- nl -- number lines of text
- printenv -- print environment variables and their values
- pwd -- print the current working directory
- rest -- tool for making rest requests with JSON
- rev -- reverse text, line by line
- rmdir -- remove a directory from the file system
- scrub -- overwrites files using the 35 pass Gutmann technique
- seq -- write a sequence of numbers to standard output
- shell -- command shell
- sleep -- sleep for a given number of seconds
- snek -- hungry snake simulation game
- spell -- checks the spelling of words
- sponge -- soak up standard input and write to a file
- stat -- print file statistics
- sum -- compute a BSD checksum of the input
- sync -- sync file system contents to persistent storage
- tac -- concatenate and print files in reverse
- tail -- prints lines at the end of an input text
- tee -- copy characters from standard input to standard output and a file
- tmpl -- template processor
- touch -- change file timestamps
- true -- exits with a success return code
- ts -- copy lines from standard input to standard output adding timestamps
- tty -- print the filename associated with the tty connected to stdin
- uname -- print system information (name and hardware details)
- unexpand -- converts spaces to tabs
- units -- convert between units of measure
- uniq -- show or hide duplicate lines in a file
- unix2dos -- replaces UNIX line endings '\n' with DOS line endings '\r\n'
- up -- universal ping, checks the status of a URL
- uptime -- show how long the system has been up
- uuidgen - COMB UUIDs (UUID v4 Variant B) generator
- wc -- counts lines, words, and characters and prints the results
- which -- finds an executable in $PATH
- who -- list users currently logged in
- whoami -- prints the current user's effective username
- xkcdpass -- password generator based on the xkcd (diceware) method
- yes -- continuously outputs the same line of text over and over again

## Documentation

All commands provide a `--help` command line option which will output
a mini manual page with usage, command line options, and examples.

For example, `cmp --help` outputs the following:

```
cmp -- compare two files

usage: cmp [OPTIONS] FILE1 FILE2

  -h, --help     print help text
  -V, --version  print version and copyright info

examples:

  # compare two files ./foo and ./bar
  cmp ./foo ./bar
```

## Versioning and Releases

`tcutils` releases once a year on July 1st. The version number corresponds
to the year the software collection was released. For example, on July 1,
2038, `tcutils` `v2038` will be released.

## Conformance

None of this software has been designed to conform to any standard. Any
resemblance to standards compliant software is purely coincidental.

## Contributing

The author is not open to external contributions at this time.
Feel free to report bugs or suggest new features.

## License

SPDX: `GPL-3.0-or-later`

See the file `COPYING` for the full license. Each file in the repository should
include its own license information.

<!--
  README.md
  Copyright 2022  Thomas Cort

  Copying and distribution of this file, with or without modification, are
  permitted in any medium without royalty, provided the copyright notice and
  this notice are preserved. This file is offered as-is, without any warranty.
-->
