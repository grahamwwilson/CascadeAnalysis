#!/bin/sh

# Target luminosity = 400 inv fb.

# You can find the integrated luminosity of the Whizard run by searching for "corr. to luminosity" in the log file.
# These can have some uncertainities due to the quality of the MC integration.

ELMIN=$1
MUMIN=$2

echo "Using ELMIN: ${ELMIN} GeV for minimum pT for electrons"
echo "Using MUMIN: ${MUMIN} GeV for minimum pT for muons" 

module load root/6.32.2
module list
root --version
python --version
python analyze.py -h

python analyze.py -w "WWlvlv"  -t 400.0 -l 2.7575e2 -p "WF-V2" -e ${ELMIN} -m ${MUMIN}
python analyze.py -w "ZZllvv"  -t 400.0 -l 3.0904e3 -p "WF-V2" -e ${ELMIN} -m ${MUMIN}
python analyze.py -w "ZZtau2l"  -t 400.0 -l 2.6543e3 -p "." -e ${ELMIN} -m ${MUMIN}
python analyze.py -w "ZDYll"  -t 400.0 -l 6.6213e-3 -p "." -e ${ELMIN} -m ${MUMIN}
python analyze.py -w "ZDYtaultaul"  -t 400.0 -l 6.6213e-3 -p "." -e ${ELMIN} -m ${MUMIN}

module unload root/6.32.2
module list

exit
