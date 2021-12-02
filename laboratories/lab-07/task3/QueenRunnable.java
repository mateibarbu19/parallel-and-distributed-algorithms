package task3;

import java.util.concurrent.ExecutorService;
import java.util.concurrent.atomic.AtomicInteger;

public class QueenRunnable implements Runnable {
    private final int step;
    private final int N;
    private final int[] graph;
    private final AtomicInteger inQueue;
    private final ExecutorService tpe;

    /**
     * @param step
     * @param n
     * @param graph
     * @param inQueue
     * @param tpe
     */
    public QueenRunnable(final int step, final int n, final int[] graph, final AtomicInteger inQueue,
            final ExecutorService tpe) {
        this.step = step;
        N = n;
        this.graph = graph;
        this.inQueue = inQueue;
        this.tpe = tpe;
    }

    @Override
    public void run() {
        if (Sequential.N == step) {
            printQueens(graph);
        } else {
            for (int i = 0; i < Sequential.N; ++i) {
                final int[] newGraph = graph.clone();
                newGraph[step] = i;

                if (check(newGraph, step)) {
                    inQueue.incrementAndGet();
                    tpe.submit(new QueenRunnable(step + 1, N, newGraph, inQueue, tpe));
                }
            }
        }

        if (inQueue.decrementAndGet() == 0) {
            tpe.shutdown();
        }
    }

    private static boolean check(final int[] arr, final int step) {
        for (int i = 0; i <= step; i++) {
            for (int j = i + 1; j <= step; j++) {
                if (arr[i] == arr[j] || arr[i] + i == arr[j] + j || arr[i] + j == arr[j] + i)
                    return false;
            }
        }
        return true;
    }

    private static void printQueens(final int[] sol) {
        StringBuilder aux = new StringBuilder();
        for (int i = 0; i < sol.length; i++) {
            aux.append("(").append(sol[i] + 1).append(", ").append(i + 1).append("), ");
        }
        aux = new StringBuilder(aux.substring(0, aux.length() - 2));
        System.out.println("[" + aux + "]");
    }
}
