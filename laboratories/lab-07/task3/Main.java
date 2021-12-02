package task3;

import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.atomic.AtomicInteger;

public class Main {
    static int N = 4;

    public static void main(final String[] args) {
        final ExecutorService tpe = Executors.newFixedThreadPool(Runtime.getRuntime().availableProcessors());
        final AtomicInteger inQueue = new AtomicInteger(0);
        inQueue.incrementAndGet();
        final int[] graph = new int[N];
        tpe.submit(new QueenRunnable(0, N, graph, inQueue, tpe));
    }
}
