#!/bin/sh
# Target SM luminosity = 400 inv fb.

# You can find the integrated luminosity of the Whizard run by searching for "corr. to luminosity" in the log file.
# These can have some uncertainities due to the quality of the MC integration.

# For the signal samples with identical parent slepton masses, these really should 
# be exactly the same number.
# I found, 2.2861e4, 2.2940e4, 2.2878e4. So let's use the average, namely, 2.2893e4

ELMIN=$1
MUMIN=$2

echo "Using ELMIN: ${ELMIN} GeV for minimum pT for electrons"
echo "Using MUMIN: ${MUMIN} GeV for minimum pT for muons" 

module load root/6.32.2
module list
root --version
python --version
python analyze.py -h

python analyze.py -w "WZ" -t 400.0 -l 694.68 -p "WF-V2" -e ${ELMIN} -m ${MUMIN} 
python analyze.py -w "ZZ" -t 400.0 -l 918.22 -p "WF-V2" -e ${ELMIN} -m ${MUMIN}
python analyze.py -w "WWW" -t 400.0 -l 6.5239e3 -p "WF-V2" -e ${ELMIN} -m ${MUMIN}
python analyze.py -w "WWZ" -t 400.0 -l 1.0247e4 -p "WF-V2" -e ${ELMIN} -m ${MUMIN}

module unload root/6.32.2
module list

exit
