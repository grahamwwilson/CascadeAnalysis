#!/bin/bash
#
# To run do something like ./respin.sh 5.0 5.0 MyOutputDirectory
# The first two arguments are the pT thresholds, the latter is the subdirectory that is created 
# and all the existing (at end of the script) files are moved to.
#
# Remake all histograms. This should maybe migrate to a series of batch jobs.
#
# Add the electron and muon minimum pT to be accepted as input arguments 1 and 2.
# These default to (2.0, 3.0 GeV)
#
# For backwards compatibility/testing of the new C++-based implementation vs the old 
# python implementation, I have added a LANG parameter.
# This also controls the ROOT environment used.
# Hopefully this does not need to be used for very long.
#

ELMIN=${1:-2.0}
MUMIN=${2:-3.0}
COLLATEDIR=${3:-LOWESTPT}
LANG=${4:-"cpp"}

echo "Using ELMIN: ${ELMIN} GeV for minimum pT for electrons"
echo "Using MUMIN: ${MUMIN} GeV for minimum pT for muons" 
echo "Using COLLATEDIR: ${COLLATEDIR} for collating ROOT files" 
echo "Using LANG: ${LANG} for cpp or python based" 

# WZ, ZZ, WWW, WWZ
time ./redoSM.sh ${ELMIN} ${MUMIN} ${LANG}

# WZZ
time ./redoWZZ.sh ${ELMIN} ${MUMIN} ${LANG}

# ZZZ
time ./redoZZZ.sh ${ELMIN} ${MUMIN} ${LANG}

# WZZMET
time ./redoWZZMET.sh ${ELMIN} ${MUMIN} ${LANG}

# ZZZMET
time ./redoZZZMET.sh ${ELMIN} ${MUMIN} ${LANG}

# All the tau files
time ./DoTauFilesNew.sh ${ELMIN} ${MUMIN} ${LANG}

# Latest files derived from Stephen's samples
time ./redo_New4l.sh ${ELMIN} ${MUMIN} ${LANG}

# Only do 50% of the WWlvlv and ZZlvlv give issues ...
time ./redo_New4l_Mopup2.sh ${ELMIN} ${MUMIN} ${LANG}

# Assemble the Standard Model background ansatz. (Note some contributions 
# eg. WZZMET, ZZZMET, are added to the background sum, but 
# are not included explicitly as separate contributions in the plots).
time ./assembleSM.sh ${LANG}

time ./redo_TSlepSlep.sh ${ELMIN} ${MUMIN} ${LANG}

# The four Sleptons files. Once the sleptons_90 file finishes we can display 
# the standard histograms.
time ./redo_Sleptons.sh ${ELMIN} ${MUMIN} ${LANG}

# The six files with the slepton cascade models used by ATLAS.
time ./redo_ATL.sh ${ELMIN} ${MUMIN} ${LANG}

if [ -d "$COLLATEDIR" ]; then
    echo "Directory $COLLATEDIR exists. Removing histos*.root files..."
    rm -f "$COLLATEDIR"/histos*.root
else
    echo "Directory $COLLATEDIR does not exist. Creating it..."
    mkdir "$COLLATEDIR"
fi

mv histos*.root ${COLLATEDIR}
echo "histogram files moved to "${COLLATEDIR}

exit
