# Laboratory Exercises 3

## Task 1

> Description: Implement a parallelized solution for Odd-Even Transposition
> Sort.

At first, I divided the array in `P` parts. Each threads receives the part of the array from `[start, end - 1]`. (`start` and `end` indices are set to be an even number).
On each of these parts, the algorithm in the `oets` function, repeats `N` time, two stages. These are:

- Even stage: take an iterator, from `start` to `end - 2`, included, and hop
    from an even to an even index; if the number on that position is less then
     the next one, swap them;

- Odd stage: take an iterator, from `start + 1` to `end - 1`, included, and hop
    from an odd to an odd index; if the number on that position is less then the next one,
     swap them;

As one might observe, the odd stage, compares a value from index `end - 1`,
with one from `end`. This means that we compare the last element from the 
current thread's array part with the first one of the "next" thread. Doing so,
we avoid a self-containment problem, and avoided to generate in the end `P`
disjoint sorted segments.

Code sources: `oets.c`, `test_oets.sh`, `utils.h` and `Makefile`
