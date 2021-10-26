#!/bin/bash

N=1000

make strassen
make PROTECTED="-DPROTECTED" strassen_par

if [ ! -f "strassen" ]
then
    echo "The binary file \"strassen\" is missing!"
    exit 1
fi

if [ ! -f "strassen_par" ]
then
    echo "The binary file \"strassen_par\" is missing!"
    exit 2
fi

echo "=== Running Strassen's sequential algorithm ==="
(time ./strassen $N > seq.txt) 2> >(grep real | awk '{print $2}')
echo "=== Running Strassen's parallelized algorithm ==="
(time ./strassen_par $N > par.txt) 2> >(grep real | awk '{print $2}')

diff -q seq.txt par.txt
if [ $? -ne 0 ]; then
    echo "Failure: The two implementations produced two different results!"
    exit 3
fi
echo "Success: The two implementations produced the same result!"

rm -f seq.txt par.txt
rm -f strassen strassen_par
