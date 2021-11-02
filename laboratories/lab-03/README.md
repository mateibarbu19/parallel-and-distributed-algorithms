# Laboratory Exercises 3

## Task 1

> Description: Implement a parallelized solution for Odd-Even Transposition
> Sort.

At first, I divided the array in `P` parts. Each threads receives the part of the array from `[start, end - 1]`. (`start` and `end` indices are set to be an even number).
On each of these parts, the algorithm in the `oets_p` function, repeats `N` time, two stages. These are:

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

## Task 1

> Description: Implement a parallelized solution for Odd-Even Transposition
> Sort.

At first, I divided the array in `P` parts. Each threads receives the part of the array from `[start, end - 1]`. (`start` and `end` indices are set to be an even number).
On each of these parts, the algorithm in the `oets_p` function, repeats `N` time, two stages. These are:

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

## Task 2

> Description: Implement a parallelized solution for Shear Sort.

Classical implementation in which each thread sorts `N / P` lines, waits at a barrier, and then sorts the same number of columns.

Code sources: `shear.c` and `test_shear.sh`

## Task 3

> Description: Implement a parallelized solution for Merge Sort.

1. At first, we divide the array in `N / P` parts.
2. Each of these is sorted in a thread using a sequential implementation of 
    merge sort. From this point on it is clear that we only have to merge the
    `P` sorted parts of the array.
3. Instead of doing this sequentially, we can have `P / 2` threads merging two
    parts at a time.
4. Then `P / 4` threads merging the resulting sorted chunks in step 3.

Let's say we first wanted to merge the `P` sequentially sorted parts. If a
thread has it's last bit 0, it will merge it's designated array with the one
adjacent to it.
Now move on the the next least significant bit, if it is 0, then merge the
resulting sorted chunk from the last step, of length `2 * N / P`, with the
adjacent one.
In the end thread 0 will merge the last remaining sorted sub-arrays.

*It is to be noted that this implementation works correctly for any `N` and `P` regardless if they are powers of 2 or not.*

Code sources: `merge.c` and `test_merge.sh`

## Task 4

> Description: Implement a parallelized solution for searching an element in a
> sorted array.

1. Divide the array in `N / P` parts, and assign a search in them on a separate
    thread.
2. If the key is equal to any of the sub-arrays extremes, then we have found it
3. If the key is greater than the left-most element in the sub-array and
    greater the right-most element, than it could be only in this sub-array
4. Break-up the sub-array in which the element fits in `P` parts and run
    searches from each thread.
5. Repeat step 2.

Code sources: `parallel_search.c` and `test_parallel_search.sh`