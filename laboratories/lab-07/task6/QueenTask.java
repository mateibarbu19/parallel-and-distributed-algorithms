package task6;

import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.RecursiveTask;

public class QueenTask extends RecursiveTask<Boolean> {
    private final int step;
    private final int N;
    private final int[] graph;

    /**
     * @param step
     * @param n
     * @param graph
     */
    public QueenTask(final int step, final int n, final int[] graph) {
        this.step = step;
        N = n;
        this.graph = graph;
    }

    @Override
    protected Boolean compute() {
        if (!check(graph, step - 1)) {
            return false;
        }

        if (Sequential.N == step) {
            printQueens(graph);
            return true;
        }

        final List<RecursiveTask<Boolean>> tasks = new ArrayList<>();
        for (int i = 0; i < Sequential.N; ++i) {
            final int[] newGraph = graph.clone();
            newGraph[step] = i;

            final RecursiveTask<Boolean> t = new QueenTask(step + 1, N, newGraph);
            tasks.add(t);
            t.fork();
        }

        Boolean success = false;
        for (final var task : tasks) {
            success = success || task.join();
        }
        return success;
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
