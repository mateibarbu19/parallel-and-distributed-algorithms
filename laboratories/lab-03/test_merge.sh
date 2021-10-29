#!/bin/bash

make

if [ ! -f "merge" ]
then
    echo "The build for binary \"merge\" failed"
    exit 1
fi

if ./merge 64 3 | grep -q "Correct sort"; then
    echo "Correct sort for N = 64 P = 3"
else
    echo "Incorrect sort for N = 64 P = 3"  
fi

if ./merge 128 4 | grep -q "Correct sort"; then
    echo "Correct sort for N = 128 P = 4"
else
    echo "Incorrect sort for N = 128 P = 4" 
fi

if ./merge 128 7 | grep -q "Correct sort"; then
    echo "Correct sort for N = 128 P = 7"
else
    echo "Incorrect sort for N = 128 P = 7"
fi

if ./merge 512 2 | grep -q "Correct sort"; then
    echo "Correct sort for N = 512 P = 2"
else
    echo "Incorrect sort for N = 512 P = 2"
fi

if ./merge 512 7 | grep -q "Correct sort"; then
    echo "Correct sort for N = 512 P = 7"
else
    echo "Incorrect sort for N = 512 P = 7"
fi

if ./merge 1024 4 | grep -q "Correct sort"; then
    echo "Correct sort for N = 1024 P = 4"
else
    echo "Incorrect sort for N = 1024 P = 4"
fi

if ./merge 1024 6 | grep -q "Correct sort"; then
    echo "Correct sort for N = 1024 P = 6"
else
    echo "Incorrect sort for N = 1024 P = 6"
fi

if ./merge 1024 7 | grep -q "Correct sort"; then
    echo "Correct sort for N = 1024 P = 7"
else
    echo "Incorrect sort for N = 1024 P = 7"
fi