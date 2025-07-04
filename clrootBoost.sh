#!/bin/sh
#
# To compile target.cpp  
# ./clany.sh target 
# Then the executable can be executed using ./filename

# This is very similar to that used in my gpProcess repository
# for execution on the KU HPC cluster

target=$1
echo 'Compiling (with ROOT libraries): '${target}.cpp

root --version

# With Boost multiprecision option
g++ -std=c++17 -g -O3 -DUSE_BOOST_MULTIPRECISION -o ${target} ${target}.cpp `root-config --cflags` -lGenVector -lquadmath `root-config --libs`


exit
