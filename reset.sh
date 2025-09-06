#!/bin/sh

VERSION="${1:-4}"
FLAVOR="${2:-BACKGROUND}"

echo "Version set to "${VERSION}
echo "Flavor set to "${FLAVOR}

rm Ana_C*
rm AnaVersions.h
ln -s AnaVersions_V${VERSION}_${FLAVOR}.h AnaVersions.h

exit
