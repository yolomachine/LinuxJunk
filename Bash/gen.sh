#!/bin/bash

counter=1
while [ $counter -le 50 ]
do
    NEW_UUID=$(cat /dev/urandom | tr -dc 'a-zA-Z' | fold -w 4 | head -n 1)
    YEAR=$(cat /dev/urandom | tr -dc '0-9' | fold -w 256 | head -n 1 | head --bytes 4)
    MONTH=$(cat /dev/urandom | tr -dc '0-9' | fold -w 256 | head -n 1 | head --bytes 2)
    DAY=$(cat /dev/urandom | tr -dc '0-9' | fold -w 256 | head -n 1 | head --bytes 2)
    DATE=$NEW_UUID$YEAR$MONTH$DAY
    touch $DATE.txt
    ((counter++))
done
