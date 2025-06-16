#!/bin/sh
#
# Add all the different histograms for SM processes together.
#

LANGUAGE=${1:-"cpp"}
echo "Using program based on ${LANGUAGE} coding"

if [ "$LANGUAGE" == "cpp" ]; then
    module load root
elif [ "$LANGUAGE" == "python" ]; then
    module load root/6.32.2
else
    echo "Unsupported language: $LANGUAGE"
    exit 1
fi

hadd histos_New4l.root histos_ZDYll.root histos_ZDYtaultaul.root histos_WWlvlv.root histos_ZZllvv.root histos_ZZtau2l.root histos_WWtaultaul.root histos_WWltaul.root

hadd histos_SM.root histos_New4l.root histos_WZ.root histos_ZZ.root histos_AllTaus.root histos_WWW.root histos_WWZ.root histos_WZZ.root histos_ZZZ.root histos_WZZMET.root histos_ZZZMET.root

if [ "$LANGUAGE" == "cpp" ]; then
    module unload root
elif [ "$LANGUAGE" == "python" ]; then
    module unload root/6.32.2
fi

exit
