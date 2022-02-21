import java.util.List;
import java.util.concurrent.Callable;

public abstract class ReduceOperation<K, V, R> implements Callable<R> {
    protected final K key;
    protected final List<V> values;

    /**
     * @param key
     * @param values
     */
    public ReduceOperation(K key, List<V> values) {
        this.key = key;
        this.values = values;
    }

    /**
     * @return the key
     */
    public K getKey() {
        return key;
    }

    /**
     * @return the values
     */
    public List<V> getValues() {
        return values;
    }
}
