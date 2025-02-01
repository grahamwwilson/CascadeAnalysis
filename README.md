# CascadeAnalysis
Code for analyzing LHE format event generator files produced by Whizard.

## Analyze
analyze.py reads specified LHE file, fills ROOT histograms with weights based on 
assumed integrated luminosities and saves histograms to a ROOT file. 
It uses the argparse package to parse  input arguments so you can type

python3 analyze.py -h 

to see command line arguments. The code leans heavily on the FourVecDataClass.py. 
New 4-vector related functions should be put there.

## Shell scripts
There are three example shell scripts for running analyze.py on various LHE files.
Start by getting Test.sh to do what you need.
Note that both redoquick.sh and redo.sh run on the various SM files and add all 
the output files for each process together, when making histos_SM.root

## ROOT plotting macro
An example ROOT plotting macro is provided ("DiPlotX.C"). 
It assumes that you have all 7 standard root files available locally - these 
should have been produced with the same version of analyze.py.
You can find the "V1" set in the SleptonCascade directory of the group work area.
This can be customized relatively easily to plot any of the standard histograms 
inside ROOT.
eg.
> root

> .x DiPlotX.C("220","h3Code") 

will produce the 3-lepton flavor code plot with the 220 GeV LSP signal case. 
