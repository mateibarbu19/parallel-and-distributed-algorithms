#!/bin/bash

N=1000
P=2

if [ ! -f "strassen" ]
then
    echo "The binary file \"strassen\" is missing!"
    exit
fi

if [ ! -f "strassen_par" ]
then
    echo "The binary file \"strassen_par\" is missing!"
    exit
fi

./strassen $N > seq.txt
./strassen_par $N > par.txt

diff seq.txt par.txt

rm -rf seq.txt par.txt
