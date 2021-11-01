#!/bin/bash

routine () {
    echo "=== Testing for N = $1 P = $2 ==="
    (time ./merge $1 $2) 2> time.txt 1> content.txt
    echo -n "Time: "; grep real time.txt | awk '{print $2}'
    grep -q "Correct sort" content.txt
    if [ $? -eq 0 ]; then
        echo "Correct sort"
    else
        echo "Incorrect sort"  
    fi
    
    echo
    rm -f time.txt content.txt
}

Ns=(64 100 128 337 500 1000 10000 100000)
Ps=( 4   5   8   3   7    4     6      8) 

make merge

if [ ! -f "merge" ]
then
    echo "The build for binary \"merge\" failed"
    exit 1
fi

for key in "${!Ns[@]}"; do
    routine ${Ns["$key"]} ${Ps["$key"]}
done

rm -f merge