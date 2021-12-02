package task2;

import java.util.concurrent.ExecutorService;
import java.util.concurrent.atomic.AtomicInteger;

public class ColorRunnable implements Runnable {
    private final int N;
    private final int MAX_COLORS;
    private final int[][] graph;
    private final int[] colors;
    private final int step;
    private final AtomicInteger inQueue;
    private final ExecutorService tpe;

    public ColorRunnable(final int N, final int MAX_COLORS, final int[][] graph, final int[] colors, final int step,
            final AtomicInteger inQueue, final ExecutorService tpe) {
        this.N = N;
        this.MAX_COLORS = MAX_COLORS;
        this.graph = graph;
        this.colors = colors;
        this.step = step;
        this.inQueue = inQueue;
        this.tpe = tpe;
    }

    @Override
    public void run() {
        if (step == N) {
            printColors(colors);
        } else {
            // for the node at position step try all possible colors
            for (int i = 0; i < MAX_COLORS; i++) {
                final int[] newColors = colors.clone();
                newColors[step] = i;
                if (verifyColors(newColors, step, graph)) {
                    inQueue.incrementAndGet();
                    tpe.submit(new ColorRunnable(N, MAX_COLORS, graph, newColors, step + 1, inQueue, tpe));
                }
            }
        }
        if (inQueue.decrementAndGet() == 0) {
            tpe.shutdown();
        }
    }

    private static boolean verifyColors(final int[] colors, final int step, final int[][] graph) {
        for (int i = 0; i < step; i++) {
            if (colors[i] == colors[step] && isEdge(i, step, graph)) {
                return false;
            }
        }
        return true;
    }

    private static boolean isEdge(final int a, final int b, final int[][] graph) {
        for (final int[] ints : graph) {
            if (ints[0] == a && ints[1] == b) {
                return true;
            }
        }
        return false;
    }

    private static void printColors(final int[] colors) {
        final StringBuilder aux = new StringBuilder();
        for (final int color : colors) {
            aux.append(color).append(" ");
        }
        System.out.println(aux);
    }
}
