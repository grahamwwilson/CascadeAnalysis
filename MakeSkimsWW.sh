#!/bin/sh

# WW->tau nu tau nu -> taul taul 
time ./StripAndWrite -i WWtau.lhe -o WWtaultaul.lhe -c 2 -p 1 -q 1

# WW-> l 
time ./StripAndWrite -i WWtau.lhe -o WWltaul.lhe -c 1 -p 2 -q 5

exit
