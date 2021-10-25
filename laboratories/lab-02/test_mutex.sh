#!/bin/bash

routine () {
    rm -rf mutex
    make NUM_THREADS=$(nproc) PROTECTED="$1" mutex

    if [ ! -f "mutex" ]; then
        echo "The build for binary file \"mutex\" failed!"
        exit 1
    fi

    correct_res=$((2*$(nproc)))

    for i in {1..1000}; do
        diff  <(./mutex) <(echo "a = $correct_res")
        if [ $? -ne 0 ]; then
        echo "Failure: A running instance outputted a incorrect result"
            return 2
        fi
    done

    echo "Success: all running instances outputted the same correct result"
    rm -rf mutex
}

echo "=== Testing without mutex protection ==="
routine

echo "=== Testing with mutex protection ==="

routine "-DPROTECTED"
