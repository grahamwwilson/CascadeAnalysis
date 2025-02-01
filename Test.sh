#!/bin/sh
#
# This takes about 7s on my laptop.

# Target SM luminosity = 400 inv fb.
# (the test LHE file here is actually the WWW SM background file that only has 10000 events)

python3 analyze.py -w "Test" -t 400.0 -l 4.4241e3

exit
