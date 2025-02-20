#!/bin/sh
#
# This takes about 18 minutes on my laptop.
#

# Target SM luminosity = 400 inv fb.
# Scale signal to SM Lumi * 10 = 4000 inv fb.

# You can find the integrated luminosity of the Whizard run by searching for "corr. to luminosity" in the log file.
# These can have some uncertainities due to the quality of the MC integration.

# For the signal samples with identical parent slepton masses, these really should 
# be exactly the same number.
# I found, 2.2861e4, 2.2940e4, 2.2878e4. So let's use the average, namely, 2.2893e4

module load root/6.32.2
module list
root --version
python3 --version
python3 analyze.py -h

python3 analyze.py -w "Run-60" -t 400.0 -l 1.2010e3 -p "."
python3 analyze.py -w "Run-61" -t 400.0 -l 7.8305e2 -p "."
python3 analyze.py -w "Run-62" -t 400.0 -l 8.1882e2 -p "."
python3 analyze.py -w "Run-63" -t 400.0 -l 1.1850e3 -p "."
python3 analyze.py -w "Run-64" -t 400.0 -l 1.2438e3 -p "."

module unload root/6.32.2
module list

exit

