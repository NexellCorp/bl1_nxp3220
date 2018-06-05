#!/bin/bash
export LICENSELEN=`wc -l license.txt | cut -f1 -d ' '`
find . -type f \( -name '*.c' -o -name '*.S' -o -name '*.h' \) | xargs -0 ./add_license.sh
find . -type f -name *.S -print0 | xargs -0 ./add_license.sh
