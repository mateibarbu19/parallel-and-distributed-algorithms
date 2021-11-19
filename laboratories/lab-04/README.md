# Laboratory 4

## Task 0

> Description: Create how many threads your machine has available, each
> printing "Hello from thread #.".

I created two classes, `Main` and `MyThread`. In `Main` we call 
`Runtime.getRuntime().availableProcessors()`
to get the maximum number of threads.
`MyThread` implements `Runnable`, so we just instantiate an array of type
`Thread` and pass a `MyThread` instantiation to each element's constructor.

Code sources: [hello](./hello/)

## Task 1

> Description: Find the bug in the `bug1` package and fix it.

If we run the `main()` method we see the code outputs a correct result. And
shouldn't it? No, it shouldn't. It a trap! We expect race conditions we two
threads modify the same memory contents in parallel. The code is actually run
sequentially because of line `src/bug1/Main.java:22` which calls `run()`,
instead of `start()`.

Code sources: [bug1](./bug1/)

## Task 2

> Description: Find the bug in the `bug2` package and fix it.
> `bug2/MyThread.java:4`: Why does this code not block? We took the same lock twice!

It doesn't block because the current instance of `MyThread` acquired the monitor. So it passes through the second lock with ease, because `synchronized` is reentrant.

Code sources: [bug2](./bug2/)

## Task 3

> Description: Find the bug in the `bug3` package and fix it.

The problem can be traced to `MyThread.java:11`. Object `a` and `b`, which are
used as monitors, have the same same value. In Java in particular, the `String`
class has a pool of already created objects, which it recycles. This results
in `a` and `b` being a reference to the same object, and thus sharing the same
monitor.

Code sources: [bug3](./bug3/)

## Task 4

> Description: Find the bug in the `bug4` package and fix it.

In this package, the `keepRunning` flag is not `volatile`. So one each thread holds
a different value of its cache.

Code sources: [bug4](./bug4/)

## Task 5

> Description: Find the bug in the `bug5` package and fix it.

This bug is special. At first we might say that each critical section
incrementing variables `valueA` and `valueB` is protected by a different
synchronized block. So, a deadlock is impossible. If you'll look more closely
at the original implementation, you'll see that the critical section protected
by the second monitor is inside the critical section protected by the first
monitor. Judging that `MyThreadA` and `MyThreadB` use each monitor
alternatively, the deadlock is inevitable.

Fix: move the second synchronized block in each class outside the first.

Code sources: [bug5](./bug5/)

## Task 6

> Description: Find the bug in the `bug6` package and fix it.

The `getInstance` method inside the `Singleton` class is not synchronized.

Code sources: [bug6](./bug6/)

## Task 7

> Description: Parallelize the doubling of the elements in an array.

I divided the array in `N / P` parts, which are processed separately on each
thread. 

Code sources: [doubleVectorElements](./doubleVectorElements/)

## Task 8

> Description: Parallelize All-Pairs Shortest Paths, Floyd-Warshall, algorithm.

At first we notice that we should use a barrier between the steps of `k`.

At each step we use the following formula:

```
 (k+1)              (k+1)         (k+1)
d     (i)(j) = min(d     (i)(k), d     (k)(j))
```

We divide the initial `N x N` matrix in `P` matrices of size `(N / P) x N` .

Since each thread is assigned `N / P` rows we know that the values for
`d[i][j]` and `d[i][k]` are owned by the same thread. But the value of
`d[k][j]` is owned by another thread. So that thread should broadcast the value
of `d[k][j]` to the other threads.

I would like to thank [teodutu](https://github.com/teodutu), for the following ideea:

The formula for computing line `k` is:

```java
for (int j = 0; j < N; j++) {
    d[k][j] = Integer.min(d[k][j], d[k][k] + d[k][j]);
}
```

But, since `d[k][k] = 0` the values for line `d[k]` stay the same.

Cite: *Introduction to Parallel Computing*, by Grama, Karypis, Kumar and
Gupta, Chapter 10.4

Code sources: [shortestPathsFloyd_Warshall](./shortestPathsFloyd_Warshall/)