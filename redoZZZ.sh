#!/bin/sh
#

# Target SM luminosity = 400 inv fb.

# You can find the integrated luminosity of the Whizard run by searching for "corr. to luminosity" in the log file.
# These can have some uncertainities due to the quality of the MC integration.

module load root/6.32.2
module list
root --version
python --version
python analyze.py -h

python analyze.py -w "ZZZ6e" -t 400.0 -l 3.0470e6 -p "WF-V2"
python analyze.py -w "ZZZ4e2mu" -t 400.0 -l 9.3569e5 -p "WF-V2"
python analyze.py -w "ZZZ2e4mu" -t 400.0 -l 9.3432e5 -p "WF-V2"
python analyze.py -w "ZZZ6mu" -t 400.0 -l 3.0365e6 -p "WF-V2"

hadd -f histos_ZZZ.root histos_ZZZ6e.root histos_ZZZ4e2mu.root histos_ZZZ2e4mu.root histos_ZZZ6mu.root

module unload root/6.32.2
module list

exit
