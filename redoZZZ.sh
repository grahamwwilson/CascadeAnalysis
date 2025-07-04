#!/bin/sh
#

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
    echo "Using C++ based ROOT/C++ code"
    CMD=./analyse3l
    module load root
elif [ "$LANGUAGE" == "python" ]; then
    echo "Using python based ROOT/python code"
    module load root/6.32.2
    python --version
    CMD="python analyze.py"
else
    echo "Unsupported language: $LANGUAGE"
    exit 1
fi 

module list
root --version

${CMD} -h

${CMD} -w "ZZZ6e" -t 400.0 -l 3.0470e6 -p "WF-V2" -e ${ELMIN} -m ${MUMIN}
${CMD} -w "ZZZ4e2mu" -t 400.0 -l 9.3569e5 -p "WF-V2" -e ${ELMIN} -m ${MUMIN}
${CMD} -w "ZZZ2e4mu" -t 400.0 -l 9.3432e5 -p "WF-V2" -e ${ELMIN} -m ${MUMIN}
${CMD} -w "ZZZ6mu" -t 400.0 -l 3.0365e6 -p "WF-V2" -e ${ELMIN} -m ${MUMIN}

hadd -f histos_ZZZ.root histos_ZZZ6e.root histos_ZZZ4e2mu.root histos_ZZZ2e4mu.root histos_ZZZ6mu.root

if [ "$LANGUAGE" == "cpp" ]; then
    module unload root
elif [ "$LANGUAGE" == "python" ]; then
    module unload root/6.32.2
fi

module list

exit
