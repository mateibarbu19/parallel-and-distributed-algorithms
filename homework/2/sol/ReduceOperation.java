import java.util.List;
import java.util.concurrent.Callable;

public abstract class ReduceOperation<K2, V2, V3> implements Callable<List<V3>> {
    protected final K2 key;
    protected final List<V2> values;

    /**
     * @param key
     * @param values
     */
    public ReduceOperation(final K2 key, final List<V2> values) {
        this.key = key;
        this.values = values;
    }

    /**
     * @return the key
     */
    public K2 getKey() {
        return key;
    }

    /**
     * @return the values
     */
    public List<V2> getValues() {
        return values;
    }
}
