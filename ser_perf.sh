#!/bin/bash

for i in $(seq 1 $1)
do
    random_number=$((1 + RANDOM % 4))
    if [[ $2 == "FIFO" ]]
    then
        ./build/ser_perf "$2" "/tmp/prog.$2" "/tmp/prog$random_number.$2"
    elif [[ $2 == "SOCKET" ]] || [[ $2 == "SHM" ]]
    then
        ./build/ser_perf "$2" "/tmp/prog$random_number.$2" "client"
    fi
done