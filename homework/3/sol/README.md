# Homework 3

Note: Am scris README-ul în engleză pentru ca vreau să postez tema o pun pe
GitHub. Dacă este vreo problemă în acest sens vă rog să mă contactați.

## The Topology

The solution is a software program that is comprised of a hierarchy of clusters
that process arrays. A cluster is made up of one coordinator and several worker
processes. There are three clusters in total and each with a coordinator.

Communication is done in the following manner: the worker processes communicates
only with their coordinator, and a coordinator with any other coordinators.

The first coordinator divides a vector among all the coordinators, which
further divide the work evenly between all workers.

## Entities

The root of this class hierarchy is the (mother class) [`Process`](process.hpp).
It stands for what any of it's subclasses should do. It holds the topology of
the distributed system and a data `buffer`. This is because all processes need
to call `print_topology`. The `start`ing point of each process is the
corresponding method which is purely virtual.

The subclass [`Coordinator`](coordinator.hpp) has only two tasks: to establish
the topology and manage the data of its own cluster. The `establish_topology`
method reads the workers, sends the topology to all connected nodes and displays
it. Then, the data is distributed and collected in chunks corresponding to
cluster sizes and worker equity.

The subclass [`Worker`](worker.hpp) displays the topology and  obtains the data
from the coordinator, does the calculations and sends it back to the coordinator
it has. It acquires the typology, prints it, receive it's sub-vector chunk,
processes it and sends it back.

Besides this I feel the code is pretty self-explanatory, and that, besides the
general design, not many design patterns have been implemented or any software
architecture.

### Implementation notes

I observe that whenever a process wants to send to itself a array of say 200
count, it blocks, a undetermined, possibly infinite, amount of time.