#!/bin/sh
#
# This took about 7s on my laptop.

# Target SM luminosity = 400 inv fb.
# (the test LHE file here is actually the WWW SM background file that only has 10000 events)

# The ROOT build 6.32.02 is in a conda environment. (It is called 6.32.2 on the system)

module load root/6.32.2
module list
root --version
python --version
python analyze.py -h

python analyze.py -w "Test" -t 400.0 -l 4.4241e3

module unload root/6.32.2
module list

exit
