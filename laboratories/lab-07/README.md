# Laboratory 7

Terminology note: see
[Sequential algorithm](https://en.wikipedia.org/wiki/Sequential_algorithm).

## Task 1

> Description: Parallelize the path finding between two nodes in a graph, using
> ExecutorService.

This would seem like a typical fork/join problem. But that's not the case.
You see, the [accepting state](task1/MyRunnable.java#L22) doesn't provide any
feedback to parent states. So this is only a parallelized backtracking.

Any time we want to advance in the backtracking by adding another value to the
possible partial solution, we first check that task and then add it the thread
pool.

It is important to note that because there is no feedback to the originator
task, there is no one to keep track of what subtasks finish or not. So this
means we don't know when to `shutdown` the `ThreadPool`.

To solve this problem we use a counter, `inQueue`, which retains how many task
are in the thread pool.

Code sources:
[task1](task1)

## Task 2

> Description: Parallelize the coloring of a graph, using ExecutorService.

Same explication as the last task.

Code sources:
[task2](task2)

## Task 3

> Description: Parallelize the N Queens problem solver, using ExecutorService.

Same explication as the last task.

Code sources:
[task3](task3)

## Task 4

> Description: Parallelize the path finding between two nodes in a graph, using
> ForkJoinPool.

This program faces a different challenge, not only to output all the solutions,
but to inform the user whether or not the algorithm was able to produce at least
one valid solution. (The same idea would be to count the number of solutions.)

Here, *we need* to use `ForkJoinPool`.

![fjp](https://ocw.cs.pub.ro/courses/_media/apd/laboratoare/forkjoin.png "FJP")

We do need not use a internal counter of the running tasks, for we know for sure
all the subtasks will be completed when the originator task is.

Code sources:
[task4](task4)

## Task 5

> Description: Parallelize the coloring of a graph, using ForkJoinPool.

Same explication as the last task.

Code sources:
[task5](task5)

## Task 6

> Description: Parallelize the N Queens problem solver, using ForkJoinPool.

Same explication as the last task.

Code sources:
[task6](task6)

## Task 7

> Description: Parallelize the value query on a Binary Search Tree, using
> ExecutorService and CompletableFuture.

Because we don't need feedback to originator tasks, `ExecutorService` is the
right tool. In order to store the result of the computation outside in a shared
variable with other classes we use `CompletableFuture`.

Code sources:
[task7](task7)

## Task 8

> Description: Parallelize the height computation of a Binary Search Tree, using
> ForkJoinPool and Future.

The main logic is this: every node creates two subtasks that each return their
height. Because of this feedback we use `ForkJoinPool`. The use of `Future`
removes the busy waiting style previously
[illustrated](task4/Main.java#L20)
when using  `ForkJoinPool`.

Code sources:
[task8](task8)
