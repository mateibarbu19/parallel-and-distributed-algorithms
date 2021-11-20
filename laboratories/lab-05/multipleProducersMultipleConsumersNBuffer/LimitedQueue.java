package multipleProducersMultipleConsumersNBuffer;

import java.util.LinkedList;

/**
 * DO NOT MODIFY
 */
public class LimitedQueue<T> extends LinkedList<T> {
    private final int limit;

    public LimitedQueue(final int limit) {
        this.limit = limit;
    }

    @Override
    public boolean add(final T o) {
        final boolean added = super.add(o);
        while (added && size() > limit) {
            super.remove();
        }
        return added;
    }

}
