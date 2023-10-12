#!/bin/bash

# This script is used to test the correctness of your program.

for part in {1..5}
do
    echo "Test Part ${part}:"

    for testcase in {1..2}
    do
        echo "Test Case ${testcase}:"
        ./hw2_110550075 < "testcase/part_${part}/t${testcase}.txt" | tee ./tmp/log | diff - "answer/part_${part}/a${testcase}.txt"

        if [ $? -eq 0 ]
        then
            echo "passed"
        else
            echo "Wrong!"
            echo "here is testcase"
            cat ./tmp/log
        fi
    done
done