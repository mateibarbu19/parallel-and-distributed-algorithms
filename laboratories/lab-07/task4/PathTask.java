package task4;

import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.RecursiveTask;

public class PathTask extends RecursiveTask<Boolean> {
    private final ArrayList<Integer> partialPath;
    private final Integer destination;
    private final int[][] graph;

    /**
     * @param partialPath
     * @param destination
     * @param graph
     */
    public PathTask(final ArrayList<Integer> partialPath, final Integer destination, final int[][] graph) {
        this.partialPath = partialPath;
        this.destination = destination;
        this.graph = graph;
    }

    @Override
    protected Boolean compute() {
        final Integer lastNode = partialPath.get(partialPath.size() - 1);
        if (lastNode == destination) {
            System.out.println(partialPath);
            return true;
        }

        final List<RecursiveTask<Boolean>> tasks = new ArrayList<>();
        // verify all the nodes to avoid cycles
        for (final int[] ints : graph) {
            if (ints[0] == lastNode && !partialPath.contains(ints[1])) {
                final ArrayList<Integer> newPartialPath = new ArrayList<>(partialPath);
                newPartialPath.add(ints[1]);

                final RecursiveTask<Boolean> t = new PathTask(newPartialPath, destination, graph);
                tasks.add(t);
                t.fork();
            }
        }

        Boolean success = false;
        for (final var task : tasks) {
            success = success || task.join();
        }
        return success;
    }
}
