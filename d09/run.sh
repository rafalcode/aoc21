#!/bin/bash
# this script does what?
EXPECTED_ARGS=0 # change value to suit!
# some quick "argument accounting"
if [ $# -ne $EXPECTED_ARGS ]; then
        echo "Correct usage: $0 <args ..>"
        exit
fi
./c 3,4,3,1,2
