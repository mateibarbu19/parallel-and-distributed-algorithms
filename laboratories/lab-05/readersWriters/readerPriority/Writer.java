package readersWriters.readerPriority;

import java.util.concurrent.BrokenBarrierException;

public class Writer extends Thread {
    private final int id;

    public Writer(final int id) {
        super();
        this.id = id;
    }

    @Override
    public void run() {
        try {
            Main.barrier.await();
        } catch (InterruptedException | BrokenBarrierException e) {
            e.printStackTrace();
        }

        try {
            Main.roomEmpty.acquire();

            try {
                System.out.println("Writer " + id + " is writing");
                Thread.sleep(100);
            } catch (final InterruptedException e) {
                e.printStackTrace();
            }

            Main.roomEmpty.release();
            Main.hasWritten[id] = true;
        } catch (final InterruptedException e) {
            System.out.println("[Writer]: I was kicked out waiting for the room to empty.");
            e.printStackTrace();
        }
    }
}
