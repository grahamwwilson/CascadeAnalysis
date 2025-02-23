#!/bin/bash
#
# Remake all histograms. This should maybe migrate to a series of batch jobs.
#

# WZ, ZZ, WWW, WWZ
time ./redoSM.sh

# WZZ
time ./redoWZZ.sh

# ZZZ
time ./redoZZZ.sh

# WZZMET
time ./redoWZZMET.sh

# ZZZMET
time ./redoZZZMET.sh

# All the tau files
time ./DoTauFiles.sh

# Assemble the Standard Model background ansatz. (Note some contributions eg. WZZMET, ZZZMET, are added to the background sum, but 
# are not included explicitly as separate contributions in the plots).
time ./assembleSM.sh

# The four Sleptons files. Once the sleptons_90 file finishes can display the standard histograms.
time ./redo_Sleptons.sh

exit
