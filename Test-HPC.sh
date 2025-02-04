#!/bin/sh
#
# This took about 7s on my laptop.

# Target SM luminosity = 400 inv fb.
# (the test LHE file here is actually the WWW SM background file that only has 10000 events)

# On the KU HPC the base OS contains both python3.9 and python3.11. 
# python and python3 point to python3.9
# but the ROOT build (at least for 6.32.08) detects python3.11 - and so need to use 3.11 
# for these applications.

module load root/6.32.08
module list
root --version

python3.11 analyze.py -h
python3.11 analyze.py -w "Test" -t 400.0 -l 4.4241e3

module unload root/6.32.08
module list

exit
