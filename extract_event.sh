#!/bin/bash

# Check if the event number is provided
if [ $# -ne 2 ]; then
    echo "Usage: $0 <lhe_file> <event_number>"
    exit 1
fi

lhe_file=$1
event_number=$2

# Extract the event using Perl to handle multi-line matching
perl -00 -ne 'print if /<event>/../\/event>/ && $. == '$event_number'' "$lhe_file"

exit
