#!/bin/bash

routine () {
    echo "=== Testing for N = $1 P = $2 ==="
    (time ./oets $1 $2) 2> time.txt 1> content.txt
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

Ns=(100 100 100 100 500 1000 1000 1000)
Ps=(  3   5   6   7   7    4    6    8) 

make oets

if [ ! -f "oets" ]
then
    echo "The build for binary \"oets\" failed"
    exit 1
fi

for key in "${!Ns[@]}"; do
    routine ${Ns["$key"]} ${Ps["$key"]}
done

rm -f oets