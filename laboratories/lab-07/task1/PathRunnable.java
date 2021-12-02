package task1;

import java.util.ArrayList;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.atomic.AtomicInteger;

public class PathRunnable implements Runnable {
    private final ArrayList<Integer> partialPath;
    private final Integer destination;
    private final int[][] graph;
    private final AtomicInteger inQueue;
    private final ExecutorService tpe;

    public PathRunnable(final ArrayList<Integer> partialPath, final Integer destination, final int[][] graph,
            final AtomicInteger inQueue, final ExecutorService tpe) {
        this.partialPath = partialPath;
        this.destination = destination;
        this.graph = graph;
        this.inQueue = inQueue;
        this.tpe = tpe;
    }

    @Override
    public void run() {
        final Integer lastNode = partialPath.get(partialPath.size() - 1);
        if (lastNode == destination) {
            System.out.println(partialPath);
        }
        else {
            // verify all the nodes to avoid cycles
            for (final int[] ints : graph) {
                if (ints[0] == lastNode && !partialPath.contains(ints[1])) {
                    final ArrayList<Integer> newPartialPath = new ArrayList<>(partialPath);
                    newPartialPath.add(ints[1]);
                    inQueue.incrementAndGet();
                    tpe.submit(new PathRunnable(newPartialPath, destination, graph, inQueue, tpe));
                }
            }
        }
        if(inQueue.decrementAndGet() == 0) {
            tpe.shutdown();
        }
    }
}
