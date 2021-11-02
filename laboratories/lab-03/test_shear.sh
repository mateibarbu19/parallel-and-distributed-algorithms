#!/bin/bash

routine () {
    echo "=== Testing for N = $1 P = $2 ==="
    if ./shear $1 $2 | grep -q "Correct sort"; then
        echo "Correct sort"
        echo -n "Time for scalable code: ";
        (time ./shear-scalar $1 $2 > /dev/null)  2> >(grep real | awk '{print $2}')
    else
        echo "Incorrect sort"  
    fi
    
    echo
}

Ls=(10 10 10 10 50 100 100 100)
Ps=( 3  5  6  7  7   4   6   8) 

make shear
make shear-scalar

if [ ! -f "shear" ]
then
    echo "The build for binary \"shear\" failed"
    exit 1
fi

if [ ! -f "shear-scalar" ]
then
    echo "The build for binary \"shear-scalar\" failed"
    exit 2
fi

for key in "${!Ls[@]}"; do
    routine ${Ls["$key"]} ${Ps["$key"]}
done

rm -f shear shear-scalar