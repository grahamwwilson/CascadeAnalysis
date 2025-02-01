#!/bin/sh

# This takes about 3 minutes on my laptop.

# Target SM luminosity = 400 inv fb.
# Scale signal to SM Lumi * 10 = 4000 inv fb.

# You can find the integrated luminosity of the Whizard run by searching for "corr. to luminosity" in the log file.
# These can have some uncertainities due to the quality of the MC integration.

# For the signal samples with identical parent slepton masses, these really should 
# be exactly the same number.
# I found, 2.2861e4, 2.2940e4, 2.2878e4. So let's use the average, namely, 2.2893e4

#python3 analyze.py -w "Sleptons-LSP220" -t 4.0e3 -l 2.2893e4
#python3 analyze.py -w "Sleptons-LSP270" -t 4.0e3 -l 2.2893e4
#python3 analyze.py -w "Sleptons-LSP260" -t 4.0e3 -l 2.2893e4

# Quicker case. Do only 100k events for each, and correspondingly reduce the integrated lumi of the analyzed data-set.
python3 analyze.py -w "Sleptons-LSP220" -n 100000 -t 4.0e3 -l 2.2893e3
python3 analyze.py -w "Sleptons-LSP270" -n 100000 -t 4.0e3 -l 2.2893e3
python3 analyze.py -w "Sleptons-LSP260" -n 100000 -t 4.0e3 -l 2.2893e3

#python3 analyze.py -w "LatestSleptons-MarchBR" -t 4.0e3 -l 2.2879e3
python3 analyze.py -w "WZBoth" -t 400.0 -l 79.432
python3 analyze.py -w "ZZ" -t 400.0 -l 250.24
python3 analyze.py -w "WWW" -t 400.0 -l 4.4241e3

hadd -f histos_SM.root histos_WZBoth.root histos_ZZ.root histos_WWW.root 

exit
