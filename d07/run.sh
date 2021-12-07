#!/bin/bash
# this script does what?
EXPECTED_ARGS=0 # change value to suit!
# some quick "argument accounting"
if [ $# -ne $EXPECTED_ARGS ]; then
        echo "Correct usage: $0 <args ..>"
        exit
fi
cat p.t | xargs ./b
