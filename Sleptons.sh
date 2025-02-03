#!/bin/sh
#
# This takes about 5 minutes on my laptop for 1M events
#

# Target SM luminosity = 400 inv fb.
# Scale this much larger signal to SM Lumi = 400 inv fb.

# You can find the integrated luminosity of the Whizard run by searching for "corr. to luminosity" in the log file.
# These can have some uncertainities due to the quality of the MC integration.

# The cross-section for this mslep/msnu=130/115 GeV sample is 27.4 times higher than that for mslep/msnu=300/289 GeV.
python3 analyze.py -w "Sleptons-LSP90" -t 4.0e2 -l 8.3681e2

exit
