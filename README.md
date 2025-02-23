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
the output files for each process together, when making histos_SM.root.
The latest iteration is respin.sh that runs everything.

## ROOT plotting macro
An example ROOT plotting macro is provided ("DiPlotXP.C"). 
It assumes that you have all the needed root files available locally - these 
should have been produced with the same version of analyze.py, and hopefully copied to 
a convenient work area (eg. your laptop).
You can find the "V1" set in the SleptonCascade directory of the group work area.
This can be customized relatively easily to plot any of the standard histograms 
inside ROOT. The most recent ones are produced directly in my CascadeAnalysis directory 
on the HPC.
eg.
> root

> .x DiPlotXP.C("90","hnleptons",1.0e9) 

will produce the lepton multiplicity plot with the 90 GeV LSP signal case. 
