#!/bin/sh

# generate a list of one line program descriptions
for SRC in $(../bin/ls -1 ../src | sort)
do
	PROG=$(../bin/echo ${SRC} | ../bin/extract '([^\.]+)\.c$' '$1')
	../bin/echo -n -- "- "
	../bin/${PROG} --help | ../bin/head -n 1
done
