#!/bin/bash

for file in *.txt; do
    if [[ $file =~ ^([A-Za-z]{4})([0-9]{4})([0-9]{4}.txt) ]]
    then
	mv $file ${BASH_REMATCH[1]^^}$(echo $(( ($(echo ${BASH_REMATCH[2]} | sed 's/^0*//') + 1) % 10000 )) | awk '{printf "%04d\n", $0;}')${BASH_REMATCH[3]}
    fi
done
