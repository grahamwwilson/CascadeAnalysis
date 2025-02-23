#!/bin/sh

# Target luminosity = 400 inv fb.

# You can find the integrated luminosity of the Whizard run by searching for "corr. to luminosity" in the log file.
# These can have some uncertainities due to the quality of the MC integration.

# For these signal samples with identical parent slepton masses, these really should 
# be exactly the same number.
# I found, 2.2861e4, 2.2940e4, 2.2878e4. So let's use the average, namely, 2.2893e4

module load root/6.32.2
module list
root --version
python --version
python analyze.py -h

python analyze.py -w "Sleptons-LSP90"  -t 400.0 -l 8.3681e2 -p "WF-V1"
python analyze.py -w "Sleptons-LSP220" -t 400.0 -l 2.2893e4 -p "WF-V1"
python analyze.py -w "Sleptons-LSP270" -t 400.0 -l 2.2893e4 -p "WF-V1"
python analyze.py -w "Sleptons-LSP260" -t 400.0 -l 2.2893e4 -p "WF-V1"

module unload root/6.32.2
module list

exit
