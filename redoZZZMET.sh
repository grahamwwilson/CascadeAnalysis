#!/bin/sh
# Target SM luminosity = 400 inv fb.

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

python analyze.py -w "ZZZMET1" -t 400.0 -l 1.0725e6 -p "WF-V2" -e ${ELMIN} -m ${MUMIN}
python analyze.py -w "ZZZMET2" -t 400.0 -l 1.0697e6 -p "WF-V2" -e ${ELMIN} -m ${MUMIN}
python analyze.py -w "ZZZMET3" -t 400.0 -l 5.4093e5 -p "WF-V2" -e ${ELMIN} -m ${MUMIN}
python analyze.py -w "ZZZMET4" -t 400.0 -l 1.0720e6 -p "WF-V2" -e ${ELMIN} -m ${MUMIN}
python analyze.py -w "ZZZMET5" -t 400.0 -l 1.0677e6 -p "WF-V2" -e ${ELMIN} -m ${MUMIN}

hadd -f histos_ZZZMET.root histos_ZZZMET1.root histos_ZZZMET2.root histos_ZZZMET3.root histos_ZZZMET4.root histos_ZZZMET5.root

module unload root/6.32.2
module list

exit
