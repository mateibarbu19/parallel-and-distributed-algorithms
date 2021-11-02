#!/bin/bash

routine () {
    echo "=== Testing for N = $1 P = $2 E = $3 ==="
    ./parallel_search $1 $2 $3
    echo -n "Time for scalable code: ";
    (time ./oets-scalar $1 $2 $3 > /dev/null)  2> >(grep real | awk '{print $2}')
    
    echo
    sleep 1
}

Ns=(100 100 100 100 100 100)
Ps=(  3   5   3   3   7   4)
Es=(  4  18  19  -1 198 201)

make parallel_search

if [ ! -f "parallel_search" ]
then
    echo "The build for binary \"parallel_search\" failed"
    exit 1
fi

for key in "${!Ns[@]}"; do
    routine ${Ns["$key"]} ${Ps["$key"]} ${Es["$key"]} 
done

rm -f parallel_search
