package readersWriters;

import java.util.concurrent.Semaphore;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

/*
 * This class implements a classic LightSwitch pattern as described by Allen B.
 * Downey in The Little Book Of Semaphores, page 70
 */
public class LightSwitch {
    private Integer counter;
    private Lock lock;
    
    public LightSwitch() {
        counter = 0;
        lock = new ReentrantLock();
    }

    public void lock(final Semaphore sem) {
        lock.lock();
        counter++;
        if (counter == 1) {
            sem.acquireUninterruptibly();
        }
        lock.unlock();
    }

    public void unlock(final Semaphore sem) {
        lock.lock();
        counter--;
        if (counter == 0) {
            sem.release();
        }
        lock.unlock();
    }
}
