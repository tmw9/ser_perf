#!/bin/bash


arg1="/tmp/prog"
if [[ $1 == "FIFO" ]]
then
    arg2="/tmp/prog.$1"
elif [[ $1 == "SOCKET" ]] || [[ $1 == "SHM" ]]
then
    arg2="server"
    if [[ $1 == "SHM" ]]
    then
        arg1="/prog"
    fi
fi
echo "HERE"

pids=()
for i in $(seq 1 4)
do
    ./build/ser_perf_worker "$1" "$arg1$i.$1" $arg2 &
    pids+=($!)
done


function killprocs() {
    for i in "${pids[@]}"
    do
        kill -9 "$i"
    done
}

trap "killprocs" SIGINT
wait