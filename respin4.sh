#!/bin/sh

VERSION=$1
echo 'Version '${VERSION}

./respin.sh 10.0 10.0 HIPT-V$VERSION}
./respin.sh 2.0 3.0 LOWESTPT-V${VERSION}
./respin.sh 5.0 5.0 LOWPT-V${VERSION}
./respin.sh 7.5 7.5 MEDPT-V${VERSION}

exit
