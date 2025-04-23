#!/bin/sh

# Z->ee, Z->mumu
time ./StripAndWrite -i ZDY.lhe -o ZDYll.lhe -c 0 -q 2

# Z->tau tau. With both decaying leptonically
time ./StripAndWrite -i ZDY.lhe -o ZDYtaultaul.lhe -c 2 -p 3 -q 3

exit
