package oneProducerOneConsumer;

public class Buffer {
    private int a;

    synchronized void put(final int value) {
        a = value;
    }

    synchronized int get() {
        return a;
    }
}
