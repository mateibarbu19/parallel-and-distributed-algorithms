#!/bin/bash

N=1000
P=$(nproc)

make multiply_seq
make multiply_inner
make multiply_middle
make PROTECTED=-DPROTECTED multiply_outer

if [ ! -f "multiply_seq" ]
then
    echo "The binary file \"multiply_seq\" is missing!"
    exit 1
fi

if [ ! -f "multiply_outer" ]
then
    echo "The binary file \"multiply_outer\" is missing!"
    exit 2
fi

if [ ! -f "multiply_middle" ]
then
    echo "The binary file \"multiply_middle\" is missing!"
    exit 3
fi

if [ ! -f "multiply_inner" ]
then
    echo "The binary file \"multiply_inner\" is missing!"
    exit 4
fi

echo "=== Running outer parallelized matrix multiplication algorithm ==="
(time ./multiply_outer $N $P > par_outer.txt) 2> >(grep real | awk '{print $2}')
sleep 1
echo "=== Running middle parallelized matrix multiplication algorithm ==="
(time ./multiply_middle $N $P > par_middle.txt) 2> >(grep real | awk '{print $2}')
sleep 1
echo "=== Running inner parallelized matrix multiplication algorithm ==="
(time ./multiply_inner $N $P > par_inner.txt) 2> >(grep real | awk '{print $2}')
sleep 1
echo "=== Running sequential matrix multiplication algorithm ==="
(time ./multiply_seq $N > seq.txt) 2> >(grep real | awk '{print $2}')
sleep 1

diff -q seq.txt par_outer.txt
if [ $? -ne 0 ]; then
    echo "Failure: multiply_outer outputted a incorrect result!"
    exit 5
fi
diff -q seq.txt par_middle.txt
if [ $? -ne 0 ]; then
    echo "Failure: multiply_middle outputted a incorrect result!"
    exit 6
fi
diff -q seq.txt par_inner.txt
if [ $? -ne 0 ]; then
    echo "Failure: multiply_inner outputted a incorrect result!"
    exit 7
fi

echo "Success: All algorithms outputted the same result"

rm -f seq.txt par_outer.txt par_middle.txt par_inner.txt
rm -f multiply_seq multiply_outer multiply_middle multiply_inner