#!/bin/sh
# Target SM luminosity = 400 inv fb.

# You can find the integrated luminosity of the Whizard run by searching for "corr. to luminosity" in the log file.
# These can have some uncertainities due to the quality of the MC integration.

# For the signal samples with identical parent slepton masses, these really should be exactly the same number.
# I found, 2.2861e4, 2.2940e4, 2.2878e4. So let's use the average, namely, 2.2893e4

module load root/6.32.2
module list
root --version
python3 --version
python3 analyze.py -h

python3 analyze.py -w "ZZZMET1" -t 400.0 -l 1.0725e6 -p "WF-V2"
python3 analyze.py -w "ZZZMET2" -t 400.0 -l 1.0697e6 -p "WF-V2"
python3 analyze.py -w "ZZZMET3" -t 400.0 -l 5.4093e5 -p "WF-V2"
python3 analyze.py -w "ZZZMET4" -t 400.0 -l 1.0720e6 -p "WF-V2"
python3 analyze.py -w "ZZZMET5" -t 400.0 -l 1.0677e6 -p "WF-V2"

hadd -f histos_ZZZMET.root histos_ZZZMET1.root histos_ZZZMET2.root histos_ZZZMET3.root histos_ZZZMET4.root histos_ZZZMET5.root
hadd -f histos_SM.root histos_WZ.root histos_ZZ.root histos_WWW.root histos_WWZ.root histos_WZZ.root histos_WZZMET.root histos_ZZZ.root histos_ZZZMET.root

module unload root/6.32.2
module list

exit
