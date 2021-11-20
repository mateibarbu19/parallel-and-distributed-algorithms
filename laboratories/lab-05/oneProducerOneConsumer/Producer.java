package oneProducerOneConsumer;

import java.util.concurrent.Semaphore;

public class Producer implements Runnable {
    private final Buffer buffer;
    static Semaphore done = new Semaphore(0);

    public Producer(final Buffer buffer) {
        this.buffer = buffer;
        done = new Semaphore(0);
    }

    @Override
    public void run() {
        for (int i = 0; i < Main.N; i++) {
            buffer.put(i);
            Producer.done.release();
            try {
                Consumer.done.acquire();
            } catch (final InterruptedException e) {
                e.printStackTrace();
            }
        }
    }

}
