#!/bin/sh

module load root/6.32.2
module list
root --version
python3 --version
python3 analyzedebug.py -h
python3 analyzedebug.py -w "Run-60" -t 400.0 -l 1.201e3 -p "."

module unload root/6.32.2
module list

exit

