#!/bin/sh

# Target SM luminosity = 400 inv fb.

# You can find the integrated luminosity of the Whizard run by searching for "corr. to luminosity" in the log file.
# These can have some uncertainities due to the quality of the MC integration.

ELMIN=$1
MUMIN=$2
LANGUAGE=${3:-"cpp"}

echo "Using ELMIN: ${ELMIN} GeV for minimum pT for electrons"
echo "Using MUMIN: ${MUMIN} GeV for minimum pT for muons" 
echo "Using program based on ${LANGUAGE} coding" 

if [ "$LANGUAGE" == "cpp" ]; then
    echo "Using C++ version"
    CMD=./analyse3l
    module load root
elif [ "$LANGUAGE" == "python" ] then
    echo "Using python version"
    python --version
    CMD=python analyze.py
    module load root/6.32.2
else
    echo "Unsupported language: $LANGUAGE"
    exit 1
fi

module list
root --version

${CMD} -h

${CMD} -w "WZZP1" -t 400.0 -l 3.9939e5 -p "WF-V2" -e ${ELMIN} -m ${MUMIN}
${CMD} -w "WZZP2" -t 400.0 -l 1.9872e5 -p "WF-V2" -e ${ELMIN} -m ${MUMIN}
${CMD} -w "WZZP3" -t 400.0 -l 3.8960e5 -p "WF-V2" -e ${ELMIN} -m ${MUMIN}
${CMD} -w "WZZP4" -t 400.0 -l 3.9234e5 -p "WF-V2" -e ${ELMIN} -m ${MUMIN}
${CMD} -w "WZZP5" -t 400.0 -l 1.9804e5 -p "WF-V2" -e ${ELMIN} -m ${MUMIN}
${CMD} -w "WZZP6" -t 400.0 -l 3.9677e5 -p "WF-V2" -e ${ELMIN} -m ${MUMIN}

${CMD} -w "WZZM1" -t 400.0 -l 6.8246e5 -p "WF-V2" -e ${ELMIN} -m ${MUMIN}
${CMD} -w "WZZM2" -t 400.0 -l 3.3438e5 -p "WF-V2" -e ${ELMIN} -m ${MUMIN}
${CMD} -w "WZZM3" -t 400.0 -l 6.6886e5 -p "WF-V2" -e ${ELMIN} -m ${MUMIN}
${CMD} -w "WZZM4" -t 400.0 -l 6.7210e5 -p "WF-V2" -e ${ELMIN} -m ${MUMIN}
${CMD} -w "WZZM5" -t 400.0 -l 3.3864e5 -p "WF-V2" -e ${ELMIN} -m ${MUMIN}
${CMD} -w "WZZM6" -t 400.0 -l 6.8287e5 -p "WF-V2" -e ${ELMIN} -m ${MUMIN}

hadd -f histos_WZZP.root histos_WZZP1.root histos_WZZP2.root histos_WZZP3.root histos_WZZP4.root histos_WZZP5.root histos_WZZP6.root
hadd -f histos_WZZM.root histos_WZZM1.root histos_WZZM2.root histos_WZZM3.root histos_WZZM4.root histos_WZZM5.root histos_WZZM6.root
hadd -f histos_WZZ.root histos_WZZP.root histos_WZZM.root

if [ "$LANGUAGE" == "cpp" ]; then
    module unload root
elif [ "$LANGUAGE" == "python" ] then
    module unload root/6.32.2
fi

module list

exit
