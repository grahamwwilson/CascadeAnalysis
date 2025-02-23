#!/bin/sh
#
# Add all the different histograms for SM processes together.
#

module load root/6.32.2

hadd -f histos_SM.root histos_WZ.root histos_ZZ.root histos_AllTaus.root histos_WWW.root histos_WWZ.root histos_WZZ.root histos_ZZZ.root histos_WZZMET.root histos_ZZZMET.root

module unload root/6.32.2

exit

