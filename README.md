# tcutils

a collection of software tools, utilities, and games.

## Description

This software package includes dozens of standalone programs that are
intended to supplement, rather than replace, the system software. The
installation instructions provide directions for installing the software
alongside the system software.

## Requirements

* C compiler and standard build tools (make, sh, ...).
* [cmake](https://cmake.org/)
* curses library (e.g. [ncurses](https://invisible-island.net/ncurses/))

## Installation

Installing in your home directory:

```
cd bin
cmake -DCMAKE_INSTALL_PREFIX:PATH=${HOME} ..
make
make install
vi ~/.profile # export PATH=${HOME}/bin:${PATH}
```

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

## Versioning

`tcutils` has numbered releases that correspond to the number of times the
collection of software has been released. Each individual program within
the collection has its own version number which follows semantic versioning.

## Conformance

None of this software has been designed to conform to any standard. Any
resemblance to standards compliant software is purely coincidental.

## Contributing

The author is not open to external contributions at this time.

## License

SPDX: `GPL-3.0-or-later`

See the file `COPYING` for the full license.

<!--
  README.md
  Copyright 2022  Thomas Cort

  Copying and distribution of this file, with or without modification, are
  permitted in any medium without royalty, provided the copyright notice and
  this notice are preserved. This file is offered as-is, without any warranty.
-->
