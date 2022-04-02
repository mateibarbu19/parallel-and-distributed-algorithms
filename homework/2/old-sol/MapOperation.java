import java.util.concurrent.Callable;

public abstract class MapOperation<K, V, R> implements Callable<R> {
    protected final K key;
    protected final V value;

    /**
     * @param key
     * @param value
     */
    public MapOperation(K key, V value) {
        this.key = key;
        this.value = value;
    }

    /**
     * @return the key
     */
    public K getKey() {
        return key;
    }

    /**
     * @return the value
     */
    public V getValue() {
        return value;
    }
}
