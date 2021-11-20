package multipleProducersMultipleConsumers;

public class Consumer implements Runnable {
    private final Buffer buffer;
    private final int id;
    private static int i = 0;

    public Consumer(final Buffer buffer, final int id) {
        this.buffer = buffer;
        this.id = id;
    }

    public int getNextI() {
        int value;
        synchronized (Consumer.class) {
            value = i;
            i++;
        }
        return value;
    }

    @Override
    public void run() {
        while (getNextI() < Main.N_PRODUCERS * Main.N) {
            synchronized (Main.results) {
                Main.results[buffer.get()]++;
            }
        }
        System.out.println("Consumer " + id + " finished.");
    }
}
