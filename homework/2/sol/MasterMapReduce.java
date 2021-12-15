import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;
import java.util.stream.Collectors;

public class MasterMapReduce<K1, V1, K2, V2, V3> {
    private final ExecutorService mapPool;
    private final ExecutorService reducePool;
    private final List<MapOperation<K1, V1, K2, V2>> mapTasks;
    private final List<ReduceOperation<K2, V2, V3>> reduceTasks;

    MasterMapReduce(final int numberOfWorkers) {
        mapPool = Executors.newFixedThreadPool(numberOfWorkers);
        reducePool = Executors.newFixedThreadPool(numberOfWorkers);
        mapTasks = new LinkedList<>();
        reduceTasks = new LinkedList<>();
    }

    /**
     * @return the mapTasks
     */
    public List<MapOperation<K1, V1, K2, V2>> getMapTasks() {
        return mapTasks;
    }

    /**
     * @return the reduceTasks
     */
    public List<ReduceOperation<K2, V2, V3>> getReduceTasks() {
        return reduceTasks;
    }

    public Map<K2, List<V2>> runMapPhase() {
        final List<Future<List<Entry<K2, V2>>>> futureMapResults = new LinkedList<>();

        for (final var task : mapTasks) {
            futureMapResults.add(mapPool.submit(task));
        }

        mapPool.shutdown();

        final Map<K2, List<V2>> mapResults = futureMapResults.stream().map(t -> {
            try {
                return t.get();
            } catch (InterruptedException | ExecutionException e) {
                e.printStackTrace();
            }
            return null;
        }).flatMap(l -> l.stream()).collect(
                Collectors.groupingBy(Entry::getKey, Collectors.mapping(Entry::getValue, Collectors.toList())));
        return mapResults;
    }

    public List<V3> runReducePhase() {
        final List<Future<List<V3>>> futureReduceResults = new LinkedList<>();
        for (final var task : reduceTasks) {
            futureReduceResults.add(reducePool.submit(task));
        }

        reducePool.shutdown();

        final List<V3> reduceResults = futureReduceResults.stream().map(t -> {
            try {
                return t.get();
            } catch (InterruptedException | ExecutionException e) {
                e.printStackTrace();
            }
            return null;
        }).flatMap(l -> l.stream()).collect(Collectors.toList());

        return reduceResults;
    }
}