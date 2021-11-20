package oneProducerOneConsumer;

import java.util.concurrent.Semaphore;

public class Consumer implements Runnable {
    private final Buffer buffer;
    static Semaphore done = new Semaphore(0);

    public Consumer(final Buffer buffer) {
        this.buffer = buffer;
    }

    @Override
    public void run() {
        for (int i = 0; i < Main.N; i++) {
            try {
                Producer.done.acquire();
            } catch (final InterruptedException e) {
                e.printStackTrace();
            }
            final int value = buffer.get();
            Consumer.done.release();
            if (value != i) {
                System.out.println("Wrong value. I was supposed to get " + i + " but I received " + value);
                System.exit(1);
            }
        }
        System.out.println("I finished Correctly");
    }

}
