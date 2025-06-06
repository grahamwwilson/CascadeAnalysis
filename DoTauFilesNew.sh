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

ELMIN=$1
MUMIN=$2

echo "Using ELMIN: ${ELMIN} GeV for minimum pT for electrons"
echo "Using MUMIN: ${MUMIN} GeV for minimum pT for muons" 

module load root/6.32.2
module list
root --version
python3 --version
python3 analyze.py -h

# Use new OneHad files with generator-level veto on more than 3 leptons (no pT or eta cuts)
#python3 analyze.py -w "Run-61-OneHad-LMAX3" -t 400.0 -l 7.8305e2 -p "." -e ${ELMIN} -m ${MUMIN}
#python3 analyze.py -w "Run-62-OneHad-LMAX3" -t 400.0 -l 8.1882e2 -p "." -e ${ELMIN} -m ${MUMIN}
# Increase weight by 1/0.96 to compensate for 4-lepton veto effect (4% of selected events fail the 4-lepton veto - and they should be there). 
python3 analyze.py -w "Run-61-OneHad-LMAX3" -t 400.0 -l 8.1568e2 -p "." -e ${ELMIN} -m ${MUMIN}
python3 analyze.py -w "Run-62-OneHad-LMAX3" -t 400.0 -l 8.5294e2 -p "." -e ${ELMIN} -m ${MUMIN}
python3 analyze.py -w "Run-60" -t 400.0 -l 1.2010e3 -p "." -e ${ELMIN} -m ${MUMIN}
python3 analyze.py -w "Run-61" -t 400.0 -l 7.8305e2 -p "." -e ${ELMIN} -m ${MUMIN}
python3 analyze.py -w "Run-62" -t 400.0 -l 8.1882e2 -p "." -e ${ELMIN} -m ${MUMIN}
python3 analyze.py -w "Run-63" -t 400.0 -l 1.1850e3 -p "." -e ${ELMIN} -m ${MUMIN}
python3 analyze.py -w "Run-64" -t 400.0 -l 1.2438e3 -p "." -e ${ELMIN} -m ${MUMIN}

hadd -f histos_SundryTaus.root histos_Run-62.root histos_Run-63.root histos_Run-64.root
hadd -f histos_HadTaus.root histos_Run-61-OneHad-LMAX3.root histos_Run-62-OneHad-LMAX3.root
hadd -f histos_AllTaus.root histos_Run-60.root histos_Run-61.root histos_SundryTaus.root histos_HadTaus.root

module unload root/6.32.2
module list

exit
