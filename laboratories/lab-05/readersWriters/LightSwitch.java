package readersWriters;

import java.util.concurrent.Semaphore;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

/*
 * This class implements a classic LightSwitch pattern as described by Allen B.
 * Downey in The Little Book Of Semaphores, page 70
 */
public class LightSwitch {
    private static Integer counter = 0; 

    public void lock(final Semaphore sem) {
        synchronized (LightSwitch.counter) {
            counter++;
            if (counter == 1) {
                sem.acquireUninterruptibly();
            }
        }
    }

    public void unlock(final Semaphore sem) {
        synchronized (LightSwitch.counter) {
            counter--;
            if (counter == 0) {
                sem.release();
            }
        }
    }
}
