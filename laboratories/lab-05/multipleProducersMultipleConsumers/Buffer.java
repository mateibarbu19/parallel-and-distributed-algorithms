package multipleProducersMultipleConsumers;

import java.util.concurrent.Semaphore;

public class Buffer {
    private int a;
    private final Semaphore empty;
    private final Semaphore full;

    Buffer() {
        empty = new Semaphore(1);
        full = new Semaphore(0);
    }

    public void put(final int value) {
        try {
            empty.acquire();
        } catch (final InterruptedException e) {
            e.printStackTrace();
        }
        a = value;
        full.release();
    }

    public int get() {
        try {
            full.acquire();
        } catch (final InterruptedException e) {
            e.printStackTrace();
        }
        final int tmp = a;
        empty.release();
        return tmp;
    }
}
