package readersWriters.writerPriority;

import java.util.concurrent.BrokenBarrierException;

import readersWriters.LightSwitch;

public class Writer extends Thread {
    private final int id;
    private final LightSwitch writerSwitch;

    public Writer(final int id) {
        super();
        this.id = id;
        writerSwitch = new LightSwitch();
    }

    @Override
    public void run() {
        try {
            Main.barrier.await();
        } catch (InterruptedException | BrokenBarrierException e) {
            e.printStackTrace();
        }

        writerSwitch.lock(Main.noReaders);
        try {
            Main.noWriters.acquire();
            Main.currentWriters++;

            try {
                System.out.println("Writer " + id + " is writing");
                Thread.sleep(100);
            } catch (final InterruptedException e) {
                e.printStackTrace();
            }
            Main.currentWriters--;

            Main.noWriters.release();
            Main.hasWritten[id] = true;
        } catch (final InterruptedException e) {
            System.out.println("[Writer]: I was kicked out of no other writer segment.");
            e.printStackTrace();
        }
        writerSwitch.unlock(Main.noReaders);
    }
}
