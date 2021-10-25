#!/bin/bash


routine () {
    rm -rf barrier
    # hard coded NUM_THREADS
    make NUM_THREADS="2" PROTECTED="$1" barrier

    if [ ! -f "barrier" ]; then
        echo "The build for binary file \"barrier\" failed!"
        exit 1
    fi

    for i in {1..1000}; do
        diff --suppress-common-lines -c <(./barrier) <(printf "1\n2\n")
        if [ $? -ne 0 ]; then
            return 2
        fi
    done

    echo "Success: all running instances outputted the same correct result"
    rm -rf barrier
}

echo "=== Testing without barrier protection ==="
routine

echo "=== Testing with barrier protection ==="

routine "-DPROTECTED"
