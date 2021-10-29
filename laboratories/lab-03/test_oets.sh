#!/bin/bash

make oets

if [ ! -f "oets" ]
then
    echo "The build for binary \"oets\" failed"
    exit 1
fi

if ./oets 100 3 | grep -q "Correct sort"; then
    echo "Correct sort for N = 100 P = 3"
else
    echo "Incorrect sort for N = 100 P = 3"  
fi

if ./oets 100 5 | grep -q "Correct sort"; then
    echo "Correct sort for N = 100 P = 5"
else
    echo "Incorrect sort for N = 100 P = 5" 
fi

if ./oets 100 6 | grep -q "Correct sort"; then
    echo "Correct sort for N = 100 P = 6"
else
    echo "Incorrect sort for N = 100 P = 6"
fi

if ./oets 100 7 | grep -q "Correct sort"; then
    echo "Correct sort for N = 100 P = 7"
else
    echo "Incorrect sort for N = 100 P = 7"
fi

if ./oets 500 7 | grep -q "Correct sort"; then
    echo "Correct sort for N = 500 P = 7"
else
    echo "Incorrect sort for N = 500 P = 7"
fi

if ./oets 1000 4 | grep -q "Correct sort"; then
    echo "Correct sort for N = 1000 P = 4"
else
    echo "Incorrect sort for N = 1000 P = 4"
fi

if ./oets 1000 6 | grep -q "Correct sort"; then
    echo "Correct sort for N = 1000 P = 6"
else
    echo "Incorrect sort for N = 1000 P = 6"
fi

if ./oets 1000 7 | grep -q "Correct sort"; then
    echo "Correct sort for N = 1000 P = 7"
else
    echo "Incorrect sort for N = 1000 P = 7"
fi

rm -f oets