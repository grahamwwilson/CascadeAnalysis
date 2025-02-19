#!/bin/sh
#
#
# Target SM luminosity = 400 inv fb.
#

# You can find the integrated luminosity of the Whizard run by searching for "corr. to luminosity" in the log file.
# These can have some uncertainties due to the quality of the MC integration.

# For the signal samples with identical parent slepton masses, these really should 
# be exactly the same number. I found, 2.2861e4, 2.2940e4, 2.2878e4. So let's use the average, namely, 2.2893e4

module load root/6.32.2
module list
root --version
python3 --version
python3 analyze.py -h

python3 analyze.py -w "WZZMETP1" -t 400.0 -l 4.4002e5 -p "WF-V2"
python3 analyze.py -w "WZZMETP2" -t 400.0 -l 4.3889e5 -p "WF-V2"
python3 analyze.py -w "WZZMETP3" -t 400.0 -l 4.3736e5 -p "WF-V2"
python3 analyze.py -w "WZZMETP4" -t 400.0 -l 4.4101e5 -p "WF-V2"
python3 analyze.py -w "WZZMETP5" -t 400.0 -l 4.3676e5 -p "WF-V2"
python3 analyze.py -w "WZZMETP6" -t 400.0 -l 4.4046e5 -p "WF-V2"
python3 analyze.py -w "WZZMETP7" -t 400.0 -l 4.3883e5 -p "WF-V2"
python3 analyze.py -w "WZZMETP8" -t 400.0 -l 4.3916e5 -p "WF-V2"

python3 analyze.py -w "WZZMETM1" -t 400.0 -l 7.8173e5 -p "WF-V2"
python3 analyze.py -w "WZZMETM2" -t 400.0 -l 7.7851e5 -p "WF-V2"
python3 analyze.py -w "WZZMETM3" -t 400.0 -l 7.7648e5 -p "WF-V2"
python3 analyze.py -w "WZZMETM4" -t 400.0 -l 7.7472e5 -p "WF-V2"
python3 analyze.py -w "WZZMETM5" -t 400.0 -l 7.7536e5 -p "WF-V2"
python3 analyze.py -w "WZZMETM6" -t 400.0 -l 7.7795e5 -p "WF-V2"
python3 analyze.py -w "WZZMETM7" -t 400.0 -l 7.7815e5 -p "WF-V2"
python3 analyze.py -w "WZZMETM8" -t 400.0 -l 7.7969e5 -p "WF-V2"

hadd -f histos_WZZMETP.root histos_WZZMETP1.root histos_WZZMETP2.root histos_WZZMETP3.root histos_WZZMETP4.root histos_WZZMETP5.root histos_WZZMETP6.root histos_WZZMETP7.root histos_WZZMETP8.root 
hadd -f histos_WZZMETM.root histos_WZZMETM1.root histos_WZZMETM2.root histos_WZZMETM3.root histos_WZZMETM4.root histos_WZZMETM5.root histos_WZZMETM6.root histos_WZZMETM7.root histos_WZZMETM8.root
hadd -f histos_WZZMET.root histos_WZZMETP.root histos_WZZMETM.root
hadd -f histos_SM.root histos_WZ.root histos_ZZ.root histos_WWW.root histos_WWZ.root histos_WZZ.root histos_WZZMET.root histos_ZZZ.root

module unload root/6.32.2
module list

exit
