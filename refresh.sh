#!/bin/bash

# Sleptons 90
./redo90.sh

# SM samples excluding ZZZ
./redoSM.sh

# ZZZ
# including adding to SM histogram
./redoZZZ.sh

exit
