import java.util.List;
import java.util.Map.Entry;
import java.util.concurrent.Callable;

public abstract class MapOperation<K1, V1, K2, V2> implements Callable<List<Entry<K2, V2>>> {
    protected final K1 key;
    protected final V1 value;

    /**
     * @param key
     * @param value
     */
    public MapOperation(final K1 key, final V1 value) {
        this.key = key;
        this.value = value;
    }

    /**
     * @return the key
     */
    public K1 getKey() {
        return key;
    }

    /**
     * @return the value
     */
    public V1 getValue() {
        return value;
    }
}
