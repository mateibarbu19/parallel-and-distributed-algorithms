#!/bin/bash

if [ "$#" -ne 1 ]; then
    echo "Usage $0 task_number"
    echo "E.g. $0 1"
    exit 1
fi

BIN_DIR=bin
seq_src="$BIN_DIR/task$1/Sequential.java"
par_src="$BIN_DIR/task$1/Main.java"
shopt -s extglob
seq_obj="${seq_src/java/class}"
par_obj="${par_src/java/class}"
echo "$seq_obj"

# build
make "$par_obj"

if [ ! -f "$seq_obj" ]; then
    echo "Build for sequential source failed!"
    exit 2
fi
if [ ! -f "$par_obj" ]; then
    echo "Build for parallel source failed!"
    exit 2
fi

# run
java -cp bin/ "task$1.Main" | sort > tmp1
java -cp bin/ "task$1.Sequential" | sort > tmp2

# check difference
diff tmp1 tmp2
if [ $? -ne 0 ]; then
    echo "Incorrect result!"
else
    echo "Correct result!"
fi

# clean
rm -f tmp1 tmp2
make clean

# exit
exit 0