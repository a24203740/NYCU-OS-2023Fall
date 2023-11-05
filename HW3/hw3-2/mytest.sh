#!/bin/bash

if [ -s "./log" ]; then
    rm "./log"
fi
touch log  
for i in $(seq 1 1000)
do
    ./hw3-2 < "./testcase/case4.txt" | grep "Time" >> log
done