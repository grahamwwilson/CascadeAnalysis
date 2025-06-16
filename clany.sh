#!/bin/sh
#
# To compile target.cpp  
# ./clany.sh target 
# Then the executable can be executed using ./filename

# This is very similar to that used in my gpProcess repository
# for execution on the KU HPC cluster

target=$1
echo 'Compiling (with ROOT libraries): '${target}.cpp

module load root
module list
root --version

g++ -g -O3 -o ${target} ${target}.cpp `root-config --cflags --glibs`
#g++ -g -O3 -o ${target} ${target}.cpp `root-config --cflags` -lGenVector `root-config --libs`

module unload root

exit
