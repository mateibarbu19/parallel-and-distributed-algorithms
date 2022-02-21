# Test 2

This is a Odd-Even Transposition Sort implementation using OpenMPI in C.

Each process starts off with the value `2 * (rank + 1)`.

In the end, process 0 collects all the sorted values and prints them.
