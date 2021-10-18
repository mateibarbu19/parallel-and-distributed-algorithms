#!/bin/bash

routine () {
    test_ids=($(seq -s " " 1 $1))
    for i in "${test_ids[@]}"; do
        echo -n "Test $i: "
        (time ./task5-test $2 $3 1> /dev/null) 2> >(grep real | awk '{print $2}')
        sleep 1
    done
}

if [ $# -ne 3 ]; then
    echo "Usage: ./test_time.sh number_of_tests array_size load_factor"
    exit
fi

C=100

rm -rf task5-test
make NUM_THREADS=1 LOAD_FACTOR=$3 task5-test

echo "=== Testing with one thread: ==="
routine $1 $2 $C

rm -rf task5-test
make NUM_THREADS=$(nproc) LOAD_FACTOR=$3 task5-test

echo "=== Testing with $(nproc) threads: ==="
routine $1 $2 $C

rm -rf task5-test