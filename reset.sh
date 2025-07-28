#!/bin/sh

VERSION="${1:-2}"
echo "Version set to "${VERSION}

rm Ana_C*
rm AnaVersions.h
ln -s AnaVersions_V${VERSION}.h AnaVersions.h

exit
