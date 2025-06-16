#!/bin/sh

# Target luminosity = 400 inv fb.

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

#${CMD} -w "ZDYll"  -t 400.0 -l 6.6213e-3 -p "." -e ${ELMIN} -m ${MUMIN}
#${CMD} -w "ZDYtaultaul"  -t 400.0 -l 6.6213e-3 -p "." -e ${ELMIN} -m ${MUMIN}
# Have issues with jobs being "killed" after analyzing about 1M events for both of the following. 
# Memory leak? or an insidious floating point error for 1 event? Work-around by only looking at first half of the events for now.
${CMD} -w "WWlvlv"  -t 400.0 -l 1.37875e2 -n 500000 -p "WF-V2" -e ${ELMIN} -m ${MUMIN}
${CMD} -w "ZZllvv"  -t 400.0 -l 1.5452e3 -n 500000 -p "WF-V2" -e ${ELMIN} -m ${MUMIN}
#${CMD} -w "ZZtau2l"  -t 400.0 -l 2.6543e3 -p "." -e ${ELMIN} -m ${MUMIN}
#${CMD} -w "WWtaultaul"  -t 400.0 -l 9.9937e1 -p "." -e ${ELMIN} -m ${MUMIN}
#${CMD} -w "WWltaul"  -t 400.0 -l 9.9937e1 -p "." -e ${ELMIN} -m ${MUMIN}

# Have moved this into the assembleSM.sh script
#hadd histos_New4l.root histos_ZDYll.root histos_ZDYtaultaul.root histos_WWlvlv.root histos_ZZllvv.root histos_ZZtau2l.root WWtaultaul.root WWltaul.root

if [ "$LANGUAGE" == "cpp" ]; then
    module unload root
elif [ "$LANGUAGE" == "python" ]; then
    module unload root/6.32.2
fi

module list

exit
