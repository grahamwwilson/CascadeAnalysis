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

#python3 analyze.py -w "WWZ" -t 400.0 -l 7.9159e3 -p "WF-V1"
#python3 analyze.py -w "WZBoth" -t 400.0 -l 79.432 -p "WF-V1"
#python3 analyze.py -w "ZZ" -t 400.0 -l 250.24 -p "WF-V1"
#python3 analyze.py -w "ZZZ" -t 400.0 -l 1.4593e5 -p "WF-V1"
#python3 analyze.py -w "ZZZ" -t 1.4593e5 -l 1.4593e5 -p "WF-V1"
# Temporary fix. Set luminosity with revised cross-section guess of 27.2ab*1.2 = 32ab
python3 analyze.py -w "ZZZ" -t 400.0 -l 3.125e5 -p "WF-V1"
#python3 analyze.py -w "WWW" -t 400.0 -l 4.4241e3 -p "WF-V1"

#hadd -f histos_SM.root histos_WZBoth.root histos_ZZ.root histos_WWW.root histos_WWZ.root

#python3 analyze.py -w "Sleptons-LSP90"  -t 400.0 -l 8.3681e2 -p "WF-V1"
#python3 analyze.py -w "Sleptons-LSP220" -t 400.0 -l 2.2893e4 -p "WF-V1"
#python3 analyze.py -w "Sleptons-LSP270" -t 400.0 -l 2.2893e4 -p "WF-V1"
#python3 analyze.py -w "Sleptons-LSP260" -t 400.0 -l 2.2893e4 -p "WF-V1"

module unload root/6.32.2
module list

exit

