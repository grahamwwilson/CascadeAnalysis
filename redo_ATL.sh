#!/bin/sh

# Target luminosity = 400 inv fb.

# You can find the integrated luminosity of the Whizard run by searching for "corr. to luminosity" in the log file.
# These can have some uncertainities due to the quality of the MC integration.

# For these signal samples with identical parent slepton masses, these really should 
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

# Order prioritizes 350
python analyze.py -w "ATLAS-350-200-100"  -t 400.0 -l 4.6153e4 -p "WF-V2" -e ${ELMIN} -m ${MUMIN}

python analyze.py -w "ATLAS-250-150-100"  -t 400.0 -l 1.2003e4 -p "WF-V2" -e ${ELMIN} -m ${MUMIN}
python analyze.py -w "ATLAS-300-200-100"  -t 400.0 -l 2.4665e4 -p "WF-V2" -e ${ELMIN} -m ${MUMIN}
python analyze.py -w "ATLAS-400-200-100"  -t 400.0 -l 8.0943e4 -p "WF-V2" -e ${ELMIN} -m ${MUMIN}
python analyze.py -w "ATLAS-450-200-100"  -t 400.0 -l 1.3504e5 -p "WF-V2" -e ${ELMIN} -m ${MUMIN}
python analyze.py -w "ATLAS-550-200-100"  -t 400.0 -l 3.3610e5 -p "WF-V2" -e ${ELMIN} -m ${MUMIN}

module unload root/6.32.2
module list

exit
