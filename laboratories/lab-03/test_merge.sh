#!/bin/bash

routine () {
    echo "=== Testing for N = $1 P = $2 ==="
    if ./merge $1 $2 | grep -q "Correct sort"; then
        echo "Correct sort"
        echo -n "Time for scalable code: ";
        (time ./merge-scalar $1 $2 > /dev/null)  2> >(grep real | awk '{print $2}')
    else
        echo "Incorrect sort"  
    fi
    
    echo
}

Ns=(64 100 128 337 500 1000 10000 100000)
Ps=( 4   5   8   3   7    4     6      8) 

make merge
make merge-scalar

if [ ! -f "merge" ]
then
    echo "The build for binary \"merge\" failed"
    exit 1
fi

if [ ! -f "merge-scalar" ]
then
    echo "The build for binary \"merge-scalar\" failed"
    exit 2
fi

for key in "${!Ns[@]}"; do
    routine ${Ns["$key"]} ${Ps["$key"]}
done

rm -f merge merge-scalar