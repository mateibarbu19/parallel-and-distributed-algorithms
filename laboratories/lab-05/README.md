# Laboratory 4

*Disclaimer*: Use only `wait`/ `notify`/ `notifyAll` in synchronization mechanisms. Do not use objects that have already implemented concurrent methods.

## Task 1

> Description: Solve the Producer-consumer problem for only one producer, one
> consumer and a buffer of size one.

I took the liberty of modifying the "fixed" code given in the `Consumer` and
`Producer` classes, as opposed to solving this problem in the `Buffer` logic.

The `Consumer` first wait's for the `Producer` to be done. After that, it
`get`s the value from the `Buffer` and signals the `Producer` that he is done.
After that it compares the retrieved value with the expected one.

The `Producer` on the other hand, thinks differently. He waits for no one's
approval to put a value in the `Buffer`, after which he signals that he is
done. Because he is tired he waits for someone to consume the value before
getting back to work.

Code sources:
[oneProducerOneConsumer](./oneProducerOneConsumer/)

## Task 2.1

> Description: Solve the Producer-consumer problem for only multiple producers,
> multiple consumers and a buffer of size one.

The last solution won't do the trick in this situation. Why? Because the
intention was to order the `Consumer`-`Producer` exchanges alternatively, first
the `Producer` `put`s, then the `Consumer` `get`s. This idea would work for
multiple `Consumer`s, but only for a single `Producer`. Because it doesn't
assure that a `Producer` doesn't overwrite another `Producer`'s value.

I decided to implement the classical solution in which the buffer is protected
by two semaphores, `empty` and `full`. I feel that the code is pretty self explanatory, so you could read the source for further clarification.

Code sources:
[multipleProducersMultipleConsumers](./multipleProducersMultipleConsumers/)

## Task 2.2

> Description: Solve the Producer-consumer problem for only multiple producers,
> multiple consumers and a buffer of any size.

I order to take into account a variable size buffer, we must first nuance the
notions explained before. When we referred to the buffer being empty, what I
actually meant is that it was not full. Vice versa, when I referred to the
buffer being full, what I actually meant is that it was not empty.

- `empty` -> `notFull`
- `full`  -> `notEmpty`

We apply the same algorithm as before, just making sure that queue
modifications are synchronized.

Code sources:
[multipleProducersMultipleConsumersNBuffer](./multipleProducersMultipleConsumersNBuffer/)

## Task 3

I implemented a solution in which every even indexed philosopher first
picks up the left fork, the the right. The odd indexed ones, first pick up the
right fork, then the left.

Code sources: (./philosophersProblem/)[./philosophersProblem/]

## Task 4

> Description: Solve the Readers-writers problem with reader priority.

I implemented the solution described in the textbook course support [[1]](#1). I
believe that the code and comments are self explanatory, so feel free to read
them for further clarification.

Code sources:
[./readersWriters/readerPriority/](./readersWriters/readerPriority/)

## Task 5

I implemented the solution described in the textbook course support [[1]](#1). I
believe that the code and comments are self explanatory, so feel free to read
them for further clarification.

Code sources:
[./readersWriters/writerPriority/](./readersWriters/writerPriority/)

## Task 6

> Description: Solve the barbershop problem.

I implemented the FIFO solution described in the textbook course support
[[1]](#1). I believe that the code and comments are self explanatory, so feel
free to read them for further clarification.

Code sources: [barbershop](./barbershop/)

## References

<a id="1">[1]</a> 
Allen B. Downey, [*The Little Book of Semaphores*](https://www.greenteapress.com/semaphores/),
Green Tee Press