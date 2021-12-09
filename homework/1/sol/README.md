# Homework 1

> Description: Parallelize a genetic algorithm that solves the knapsack problem.

### First step

At first I optimized the obvious inefficient compare individuals function. I
added the `count` filed in the `individual` structure, which stores how many
one chromosomes are "1".

## The main idea

Look in `homework.c`. The function, `run_genetic_algorithm`, used to be the
"brains" of this implementation. So it was parallelized. It was the only clean
solution I thought of in order to keep all the threads alive and not join them
until the main function returned.

### Current generation

At each stage of the genetic algorithm we hold dear our `current_generation`
array of individuals. Note that it is shared with all the other threads.

Each thread receives the number of individuals in the current generation
divided by the number of threads, at first. What do I mean at first? Since at
each step we need to perform different tasks, there is no need to separate
chunks of this array and reserve them for each thread.

So, *at first*, we compute the characteristic of each thread in
`compute_characteristics`. I would like to note that at the exit point of this
function we do not wait at a barrier. *Why?* Because after each call of this
kind we call in action `p_mergesort`.

`p_mergesort` is smart. First of all it divides the array it received in
`thread_count` parts. After which, it sorts them in parallel. At the end, we
wait at a barrier and merge the sorted chunks. Until this so called "end" there
was no need to swap elements between the divided chunks. This is why we don't
wait at a barrier at the end of `compute_characteristics`, because each thread
is isolated.

All threads wait at a barrier to exit `p_mergesort`. Now when we want to mutate
the current generation and build the `next_generation` we keep thinking the same
way.

Pop quiz: *Do we need to wait at a barrier after keeping elite children*
*selection?* Trick question, because it depends, what do we do next? Just mutate
the first 20% children with the first version of bit string mutation. Sounds
like it could be done in parallel to me. No barriers until now. We are
unstoppable.

Pop quiz: *Do we need to wait at a barrier after the crossover of the first 30%*
*parents with one-point crossover?* What do we do next? We swap the current
generation with the next one. Watch out! A few steps after we reach the end of
the loop. Meaning that we will recompute the characteristics of each individual.
Notice that the crossover set includes the subarray which would be assigned to
the last thread in `compute_characteristics`. If this last thread finishes
before the rest, it may process old data. So be a good thread and wait at a
barrier here.

## Parallel Merge-Sort

```
N = number_of_individuals_in_a_generation;
P = thread_count;
```

1. At first, we divide the array in `N / P` parts.
2. Each of these is sorted in a thread using a sequential implementation of 
    merge sort. From this point on it is clear that we only have to merge the
    `P` sorted parts of the array.
3. Instead of doing this sequentially, we can have `P / 2` threads merging two
    parts at a time.
4. Then `P / 4` threads merging the resulting sorted chunks in step 3.

Let's say we first wanted to merge the `P` sequentially sorted parts. If a
thread has its last bit 0, it will merge its designated array with the one
adjacent to it.
Now move on the the next least significant bit, if it is 0, then merge the
resulting sorted chunk from the last step, of length `2 * N / P`, with the
adjacent one.
In the end thread 0 will merge the last remaining sorted sub-arrays.

*It is to be noted that this implementation works correctly for any `N` and `P`*
*regardless if they are powers of 2 or not.*

---

Since the rest of the implementation is the same code given in the initial
infrastructure, I won't explain it.

It may seem like the code is too short. Trust me it took a lot of time to catch
all the bugs, and condense it to this form. I would call it anything but an easy
implementation.

It may seem like the README is too long.

> "If I had more time, I would have written a shorter letter." - proverb
