#!/bin/bash

if [ -s "./log" ]; then
    rm "./log"
fi
touch log  
for i in $(seq 1 100)
do
    ./hw3-2-mutex < "./testcase/case5.txt" >> log
done