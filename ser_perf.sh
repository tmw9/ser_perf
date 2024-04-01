#!/bin/bash

for i in $(seq 1 $1)
do
    random_number=$((1 + RANDOM % 4))

    dim=$((1 + RANDOM % 1024))
    ./build/ser_perf "FIFO" "/tmp/prog.FIFO" "/tmp/prog$random_number.FIFO" $dim
    sleep 2
    ./build/ser_perf "SOCKET" "/tmp/prog$random_number.SOCKET" "client" $dim
    sleep 2
    ./build/ser_perf "SHM" "/prog$random_number.SHM" "client" $dim
    sleep 2
done