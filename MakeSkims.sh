#!/bin/sh

# WZ -> l nu tau+ tau-
time ./StripAndWrite -i input60.lhe -o Run-60.lhe -c 2

# ZZ -> l+l- tau+ tau- 
time ./StripAndWrite -i input61.lhe -o Run-61.lhe -c 2

# ZZ -> tau+ tau- tau+ tau-
time ./StripAndWrite -i input62.lhe -o Run-62.lhe -c 4

# WZ -> tau nu tau+ tau-
time ./StripAndWrite -i input63.lhe -o Run-63.lhe -c 3

# WZ -> tau nu l+l-
time ./StripAndWrite -i input64.lhe -o Run-64.lhe -c 1

exit
