package readersWriters.writerPriority;

import java.util.concurrent.BrokenBarrierException;

import readersWriters.LightSwitch;

public class Reader extends Thread {
    private final int id;
    private final LightSwitch readLightswitch;

    public Reader(final int id) {
        super();
        this.id = id;
        readLightswitch = new LightSwitch();
    }

    @Override
    public void run() {
        try {
            Main.barrier.await();
        } catch (InterruptedException | BrokenBarrierException e) {
            e.printStackTrace();
        }

        Main.noReaders.acquireUninterruptibly();
        readLightswitch.lock(Main.noWriters);
        Main.noReaders.release();        

        Main.currentReaders++;
        try {
            System.out.println("Reader " + id + " is reading");
            Thread.sleep(100);
        } catch (final InterruptedException e) {
            e.printStackTrace();
        }
        Main.hasRead[id] = true;
        synchronized (Main.currentReaders) {
            Main.currentReaders--;
        }
        
        readLightswitch.unlock(Main.noWriters);
    }
}
