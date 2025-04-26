#!/bin/sh

# Now use LV code!

# ZZ -> tau+ tau- tau+ tau-
time ./StripAndWriteLV -i Run-62-OneHad.lhe -o Run-62-OneHad-LVDummy.lhe -c 3

# ZZ -> tau+ tau- tau+ tau-
time ./StripAndWriteLV -i Run-62-OneHad.lhe -o Run-62-OneHad-LMAX3.lhe -c 3 -l 3

# ZZ -> l+l- tau+ tau- 
time ./StripAndWriteLV -i Run-61-OneHad.lhe -o Run-61-OneHad-LVDummy.lhe -c 1

# ZZ -> l+l- tau+ tau- 
time ./StripAndWriteLV -i Run-61-OneHad.lhe -o Run-61-OneHad-LMAX3.lhe -c 1 -l 3



exit
