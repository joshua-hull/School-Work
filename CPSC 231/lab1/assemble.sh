#!/bin/bash

if [ "$#" -lt "1" ]
then
	echo "Error: Missing argument"
	echo "Usage: assemble.sh <source_file>"
	exit 1
fi

printf "`cat pass1.m`\ninclude($1)" | m4 > symbols.m
printf "`cat pass2.m`\ninclude($1)" | m4 > executable

