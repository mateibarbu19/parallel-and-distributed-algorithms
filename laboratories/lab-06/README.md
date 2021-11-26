# Laboratory 6

## Task 1

> Description: Solve the Producer-consumer problem for multiple producers and 
> consumers, using ArrayBlockingQueue.

Maintained the same solution as in the
[last laboratory](../lab-05/multipleProducersMultipleConsumersNBuffer),
but removed the `notEmpty` and `notFull` locks. This was possible with the help
of the blocking methods `BlockingQueue`, e.g. `put` and `take`.

Code sources:
[multipleProducersMultipleConsumers](multipleProducersMultipleConsumers)

## Task 2

> Description: Solve the synchronization problem in the package with the
> corresponding name using AtomicInteger

Just removed attribution blocks with `set` calls and used the `get` method to
retrieve the stored value.

Code sources:
[synchronizationProblem](synchronizationProblem)

## Task 3

> Description: Solve the bug which appears in the dummy usage of
> ConcurrentHashMap, using the methods in that class.

Only modified the `addValue` function in `MyThread`.

At first we must make an observation. Between two subsequent method calls on
`hashMap`, it's internal state might change. So it is impossible to use a
classic `if` structure, like the initial infrastructure provided, because the
condition validated in the `if` clause can become false when entering a branch.

First check whether the `ConcurrentHashMap` has no value associated with the
respective key. This has to be done with `putIfAbsent`.
(PS: I am offering a hug for anybody discovering another method.)
If a value already existed, then simply replace it with the sum between the old
value and second parameter.

An insightful alternative provided by
[Andrei Ionescu](https://github.com/AndreiInfo)
is inserted as a comment.

Code sources:
[bugConcurrentHashMap](bugConcurrentHashMap)

## Task 4

> Description: Parallelize the program in `synchronizedSortedList`.
> Thread 0, 1, 2: read the input form a file and add it in a common list.
> Thread 3: Sort the list.
> Use a semaphore to synchronize them.

Made a 0-initialized semaphore available to all threads. Each time a reader
finishes, it releases this semaphore. The thread wants to sort the common list,
acquires the semaphore with the number of reader threads permits. 

Code sources:
[synchronizedSortedList](synchronizedSortedList)

## Task 5

> Description: Parallelize the program in `parallelTree`.
> Two threads build the binary tree in parallel.
> The third verifies the build validity.

This one follows the same idea as the previous task. Made a 0-initialized
semaphore available to all threads. Each time a reader finishes, it releases
this semaphore. The thread wants to sort the common tree, acquires the semaphore
with the number of reader threads permits. 

Code sources:
[parallelTree](parallelTree)
