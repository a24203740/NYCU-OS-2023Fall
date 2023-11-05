#!/bin/bash

if [ -s "./log" ]; then
    rm "./log"
fi
touch log  
for i in $(seq 1 100)
do
    ./hw3-2-backup < "./testcase/case6.txt" | grep "Time" >> log
done