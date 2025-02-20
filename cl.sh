#!/bin/sh
#
# To compile filename.cpp  
# ./cl.sh filename

target=StripAndWrite
echo 'Compiling : '${target}.cpp

g++ -g -o ${target} ${target}.cpp

exit

