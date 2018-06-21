#!/usr/bin/env bash

echo "Debugging core files"

shopt -s nullglob
for i in *.core; do
   echo $i
   echo "backtrace full" | ./libtool --mode execute gdb -q test-libmongoc $i
done
