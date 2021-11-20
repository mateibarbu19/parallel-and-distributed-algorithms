package oneProducerOneConsumer;

/**
 * DO NOT MODIFY
 */
public class Main {
    public static final int N = 1000000;

    public static void main(final String[] args) {
        final Buffer buffer = new Buffer();

        final Thread[] threads = new Thread[2];
        threads[0] = new Thread(new Producer(buffer));
        threads[1] = new Thread(new Consumer(buffer));

        for (int i = 0; i < 2; i++) {
            threads[i].start();
        }
        for (int i = 0; i < 2; i++) {
            try {
                threads[i].join();
            } catch (final InterruptedException e) {
                e.printStackTrace();
            }
        }
    }

}
/* DO NOT MODIFY */