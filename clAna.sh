#!/bin/sh
#
# To compile filename.cpp  
# ./clTest.sh filename

target=analyse3l
echo 'Compiling : '${target}.cpp

g++ -g -O3 -o ${target} ${target}.cpp `root-config --cflags --libs`

exit

