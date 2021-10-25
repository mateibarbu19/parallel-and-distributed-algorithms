# Laboratory Exercises 2

## Task 1

> Description: Solve the race condition in file `mutex.c`

I noticed that in function `f` if all the threads enter the critical section
simultaneously, than `a` will be 2. If all of them enter the section
sequentially, then `a` will be `2*$(nproc)`. An example of building the source
can be found in `test_mutex.sh`.

Code sources: `mutex.c`, `test_mutex.sh` and `Makefile`

## Task 2

> Description: Make sure that function `f` in `barrier.c` always outputs "1\n2"

I noticed that in function `f` if one thread lags behind another, then we have
a nondeterministic behavior. If all of them stopped before trying to print 2,
there is no way of disordering the output. An example of building the source
can be found in `test_barrier.sh`.

Code sources: `barrier.c` and `test_barrier.sh`

## Task 3, 4 and 5

> Description: Parallelize the sequential matrix multiplication algorithm.

As I improvement to the default algorithm, I added a cache tiling optimized
implementation to the comparison.
The three main loops of the algorithm are an embarrassingly parallel task. Only
when parallelizing the inner most do we need to avoid race conditions through
mutex.

Code source: `multiply_seq.c`, `multiply_inner.c`, `multiply_middle.c`, `multiply_outer.c` and `multiply_cache_tiling.c`

## Task 6

> Description: Parallelize [Strassen's algorithm.](https://en.wikipedia.org/wiki/Strassen_algorithm)



Code sources: `strassen.c`, `strassen_par.c` and `test_strassen.sh`
