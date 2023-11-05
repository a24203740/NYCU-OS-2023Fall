#!/bin/bash

if [ $# -lt 1 ]; then
    echo "Usage: hw3-1_test.sh <excutable name>"
    exit 1
fi

TMPDIR="/tmp/tmp.HW3-1"
TMPFILE="${TMPDIR}/tmp.output"


mkdir "${TMPDIR}"

./"$1" > "${TMPFILE}"

awk 'BEGIN { 
        currentLine=0; 
        FS=" "; 
        error=0;
        RED="\033[0;31m"
        NC="\033[0m"
        GREEN="\033[0;32m"
    }
    {
        if(int($3) != int(NR-1)) 
        {
            printf("%s%d:\t%s\n", RED, NR, $0)
            error=1;
        } 
        else
        {
            printf("%s%d:\t%s\n", GREEN, NR, $0)
        }
    }
    END {
        print "=========="
        if(error == 0)
        {
            printf("%scongrats! no error.\n", GREEN);
        }
        else
        {
            printf("%sgot error, sad:(\n", RED);
        }
        print "=========="
    }' $TMPFILE

rm -rf "${TMPDIR}"