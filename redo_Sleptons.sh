#!/bin/sh

# Target luminosity = 400 inv fb.

# You can find the integrated luminosity of the Whizard run by searching for "corr. to luminosity" in the log file.
# These can have some uncertainities due to the quality of the MC integration.

# For these signal samples with identical parent slepton masses, these really should 
# be exactly the same number.
# I found, 2.2861e4, 2.2940e4, 2.2878e4. So let's use the average, namely, 2.2893e4

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

${CMD} -w "Sleptons-LSP90"  -t 400.0 -l 8.3681e2 -p "WF-V1" -e ${ELMIN} -m ${MUMIN}
${CMD} -w "Sleptons-LSP220" -t 400.0 -l 2.2893e4 -p "WF-V1" -e ${ELMIN} -m ${MUMIN}
${CMD} -w "Sleptons-LSP270" -t 400.0 -l 2.2893e4 -p "WF-V1" -e ${ELMIN} -m ${MUMIN}
${CMD} -w "Sleptons-LSP260" -t 400.0 -l 2.2893e4 -p "WF-V1" -e ${ELMIN} -m ${MUMIN}

if [ "$LANGUAGE" == "cpp" ]; then
    module unload root
elif [ "$LANGUAGE" == "python" ]; then
    module unload root/6.32.2
fi

module list

exit
