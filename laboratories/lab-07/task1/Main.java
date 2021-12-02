package task1;

import java.util.ArrayList;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.atomic.AtomicInteger;

public class Main {
    static int[][] graph = { { 0, 1 }, { 0, 4 }, { 0, 5 }, { 1, 0 }, { 1, 2 }, { 1, 6 }, { 2, 1 }, { 2, 3 }, { 2, 7 },
            { 3, 2 }, { 3, 4 }, { 3, 8 }, { 4, 0 }, { 4, 3 }, { 4, 9 }, { 5, 0 }, { 5, 7 }, { 5, 8 }, { 6, 1 },
            { 6, 8 }, { 6, 9 }, { 7, 2 }, { 7, 5 }, { 7, 9 }, { 8, 3 }, { 8, 5 }, { 8, 6 }, { 9, 4 }, { 9, 6 },
            { 9, 7 } };

    public static void main(final String[] args) {
        final ArrayList<Integer> partialPath = new ArrayList<>();
        partialPath.add(0);
        final ExecutorService tpe = Executors.newFixedThreadPool(Runtime.getRuntime().availableProcessors());
        final AtomicInteger inQueue = new AtomicInteger(0);
        inQueue.incrementAndGet();
        // compute all the paths from node 0 to 3
        tpe.submit(new PathRunnable(partialPath, 3, graph, inQueue, tpe));
    }
}
