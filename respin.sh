#!/bin/bash
#
# Remake all histograms. This should maybe migrate to a series of batch jobs.
#
# Add the electron and muon minimum pT to be accepted as input arguments 1 and 2.
# These default to (2.0, 3.0 GeV)
#

ELMIN=${1:-2.0}
MUMIN=${2:-3.0}

echo "Using ELMIN: ${ELMIN} GeV for minimum pT for electrons"
echo "Using MUMIN: ${MUMIN} GeV for minimum pT for muons" 

# WZ, ZZ, WWW, WWZ
time ./redoSM.sh ${ELMIN} ${MUMIN}

# WZZ
time ./redoWZZ.sh ${ELMIN} ${MUMIN}

# ZZZ
time ./redoZZZ.sh ${ELMIN} ${MUMIN}

# WZZMET
time ./redoWZZMET.sh ${ELMIN} ${MUMIN}

# ZZZMET
time ./redoZZZMET.sh ${ELMIN} ${MUMIN}

# All the tau files
time ./DoTauFiles.sh ${ELMIN} ${MUMIN}

# Assemble the Standard Model background ansatz. (Note some contributions 
# eg. WZZMET, ZZZMET, are added to the background sum, but 
# are not included explicitly as separate contributions in the plots).
time ./assembleSM.sh

# The four Sleptons files. Once the sleptons_90 file finishes we can display 
# the standard histograms.
time ./redo_Sleptons.sh ${ELMIN} ${MUMIN}

COLLATEDIR=${3:-LOWESTPT}

mkdir ${COLLATEDIR}
mv histos*.root ${COLLATEDIR}

exit
