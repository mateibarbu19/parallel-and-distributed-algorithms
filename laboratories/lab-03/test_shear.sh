#!/bin/bash

if [ ! -f "shear" ]
then
    echo "The build for binary \"shear\" failed"
    exit 1
fi

if ./shear 10 3 | grep -q "Correct sort"; then
    echo "Correct sort for L = 10 P = 3"
else
    echo "Incorrect sort for L = 10 P = 3"  
fi

if ./shear 10 5 | grep -q "Correct sort"; then
    echo "Correct sort for L = 10 P = 5"
else
    echo "Incorrect sort for L = 10 P = 5" 
fi

if ./shear 10 6 | grep -q "Correct sort"; then
    echo "Correct sort for L = 10 P = 6"
else
    echo "Incorrect sort for L = 10 P = 6"
fi

if ./shear 10 7 | grep -q "Correct sort"; then
    echo "Correct sort for L = 10 P = 7"
else
    echo "Incorrect sort for L = 10 P = 7"
fi

if ./shear 50 7 | grep -q "Correct sort"; then
    echo "Correct sort for L = 50 P = 7"
else
    echo "Incorrect sort for L = 50 P = 7"
fi

if ./shear 100 4 | grep -q "Correct sort"; then
    echo "Correct sort for L = 100 P = 4"
else
    echo "Incorrect sort for L = 100 P = 4"
fi

if ./shear 100 6 | grep -q "Correct sort"; then
    echo "Correct sort for L = 100 P = 6"
else
    echo "Incorrect sort for L = 100 P = 6"
fi

if ./shear 100 7 | grep -q "Correct sort"; then
    echo "Correct sort for L = 100 P = 7"
else
    echo "Incorrect sort for L = 100 P = 7"
fi