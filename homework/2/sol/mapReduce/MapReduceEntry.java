package mapReduce;

import java.util.Map;

public class MapReduceEntry<K, V> implements Map.Entry<K, V> {
    private final K key;
    private V value;

    /**
     * @param key
     * @param value
     */
    public MapReduceEntry(final K key, final V value) {
        this.key = key;
        this.value = value;
    }

    @Override
    public K getKey() {
        return key;
    }

    @Override
    public V getValue() {
        return value;
    }

    @Override
    public V setValue(final V value) {
        this.value = value;
        return this.value;
    }
}
