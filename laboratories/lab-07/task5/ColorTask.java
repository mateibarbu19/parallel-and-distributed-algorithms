package task5;

import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.RecursiveTask;

public class ColorTask extends RecursiveTask<Boolean> {
    private final int N;
    private final int MAX_COLORS;
    private final int[][] graph;
    private final int[] colors;
    private final int step;

    /**
     * @param n
     * @param mAX_COLORS
     * @param graph
     * @param colors
     * @param step
     */
    public ColorTask(final int n, final int MAX_COLORS, final int[][] graph, final int[] colors, final int step) {
        N = n;
        this.MAX_COLORS = MAX_COLORS;
        this.graph = graph;
        this.colors = colors;
        this.step = step;
    }

    @Override
    protected Boolean compute() {
        if (!verifyColors(colors, step - 1, graph)) {
            return false;
        }

        if (step == N) {
            printColors(colors);
            return true;
        }

        final List<RecursiveTask<Boolean>> tasks = new ArrayList<>();
        // for the node at position step try all possible colors
        for (int i = 0; i < MAX_COLORS; i++) {
            final int[] newColors = colors.clone();
            newColors[step] = i;

            final RecursiveTask<Boolean> t = new ColorTask(N, MAX_COLORS, graph, newColors, step + 1);
            tasks.add(t);
            t.fork();
        }

        Boolean success = false;
        for (final var task : tasks) {
            success = success || task.join();
        }
        return success;
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
