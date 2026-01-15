#!/bin/sh

VERSION="${1:-4}"
FLAVOR="${2:-BACKGROUND}"
REWEIGHT="${3:-False}"

echo "Version set to "${VERSION}
echo "Flavor set to "${FLAVOR}
echo "Branching fraction reweighting set to "${REWEIGHT}

rm Ana_C*
rm AnaVersions.h
rm ReWeight.h
ln -s AnaVersions_V${VERSION}_${FLAVOR}.h AnaVersions.h
ln -s ReWeight_${REWEIGHT}.h ReWeight.h

exit
