# Laboratory 1

## Task 1

> Description: A simple introductory example into using Pthreads Library.

I noticed that function `f` is executed `NUM_THREADS` times in parallel.

Code source: `task1.c`

## Task 2

> Description: Change the workflow to use the maximum number of available threads.

I wrote a `Makefile` that can be configured via command line. Running example:

```bash
make NUM_THREADS=$(nproc) example
```

An alternative solution is to use the official hint:
```c
#include <unistd.h>
 
long cores = sysconf(_SC_NPROCESSORS_CONF);
```

Code source: `Makefile`

## Task 3

> Description: Modify function `f` to write a 100 times "Hello World"

Although we are running a multithread application, it runs concurrent with the
operating systems's other processes, meaning that it is multitasked. Therefore
we don't see the threads producing an in order sequential output stream, but
rather a shuffled one.

Code source: `task3.c`

## Task 4

> Description: Modify the code source two use only two threads.

The functions `foo` and `bar` run on two parallel threads, respectively CPU
cores. Since they are easily computable functions, they print in order a
paraphrased fragment from [Foobar](https://en.wikipedia.org/wiki/Foobar).

Code source: `task4.c`

## Task 5

> Description: Parallelize the serial incrementation of an array.

We notice that the multithread implementation does not perform better than a
sequential solution. To see an improvement I added a `LOAD_FACTOR` macro to
harden the computational effort. Without a `LOAD_FACTOR` the CPU was working
with using too much information from DRAM. Since DRAM is much slower than the
CPU frequency, each thread was mostly waiting for new data to be cached.

With a proper `LOAD_FACTOR` gets to spend more time processing data than
fetching it.

```bash
$ make NUM_THREADS=$(nproc) task5
$ ./task5 10 1
0 1 2 3 4 5 6 7 8 9
1 2 3 4 5 6 7 8 9 10
$ make NUM_THREADS=$(nproc) LOAD_FACTOR=100 task5-test
$ ./task5-test 10 1
0 1 2 3 4 5 6 7 8 9
100 101 102 103 104 105 106 107 108 109
```

Code sources: `task5.c` and `werid.c`

## Task 6

> Description: Test the previous implementation's performance.

In order to test the performance of task5, I have created a bash script with
the following usage:

> Usage: ./test_time.sh number_of_tests array_size load_factor

Example:

```bash
$ ./test_time.sh 3 1000000 100
gcc -Wall -Wextra -DLOAD_FACTOR=100 \
-DNUM_THREADS=1 -o task5-test task5.c -lpthread
=== Testing with one thread: ===
Test 1: 0m0,294s
Test 2: 0m0,297s
Test 3: 0m0,297s
gcc -Wall -Wextra -DLOAD_FACTOR=100 \
-DNUM_THREADS=8 -o task5-test task5.c -lpthread
=== Testing with 8 threads: ===
Test 1: 0m0,162s
Test 2: 0m0,168s
Test 3: 0m0,170s
```

Code sources: `test_time.sh`
