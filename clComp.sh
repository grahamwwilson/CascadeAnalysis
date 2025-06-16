#!/bin/sh
#
# To compile filename.cpp  
# ./clTest.sh filename

target=CompareHistograms
echo 'Compiling : '${target}.cpp

g++ -g -O3 -o ${target} ${target}.cpp `root-config --cflags --libs`

exit

