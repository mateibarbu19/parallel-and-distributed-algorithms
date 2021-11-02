#!/bin/bash

routine () {
    echo "=== Testing for N = $1 P = $2 ==="
    if ./oets $1 $2 | grep -q "Correct sort"; then
        echo "Correct sort"
        echo -n "Time for scalable code: ";
        (time ./oets-scalar $1 $2 > /dev/null)  2> >(grep real | awk '{print $2}')
    else
        echo "Incorrect sort"  
    fi
    
    echo
}

Ns=(100 100 100 100 500 1000 1000 1000)
Ps=(  3   5   6   7   7    4    6    8) 

make oets
make oets-scalar

if [ ! -f "oets" ]
then
    echo "The build for binary \"oets\" failed"
    exit 1
fi

if [ ! -f "oets-scalar" ]
then
    echo "The build for binary \"oets-scalar\" failed"
    exit 2
fi

for key in "${!Ns[@]}"; do
    routine ${Ns["$key"]} ${Ps["$key"]}
done

rm -f oets oets-scalar