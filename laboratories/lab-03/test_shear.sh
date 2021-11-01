#!/bin/bash

routine () {
    echo "=== Testing for L = $1 P = $2 ==="
    (time ./shear $1 $2) 2> time.txt 1> content.txt
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

Ls=(10 10 10 10 50 100 100 100)
Ps=( 3  5  6  7  7   4   6   8) 

make shear

if [ ! -f "shear" ]
then
    echo "The build for binary \"shear\" failed"
    exit 1
fi

for key in "${!Ls[@]}"; do
    routine ${Ls["$key"]} ${Ps["$key"]}
done

rm -f shear
