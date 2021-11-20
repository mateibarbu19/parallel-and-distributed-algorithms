package multipleProducersMultipleConsumersNBuffer;

import java.util.Queue;
import java.util.concurrent.Semaphore;

public class Buffer {
    private final Queue<Integer> queue;
    private final Semaphore notEmpty;
    private final Semaphore notFull;
    
    public Buffer(final int size) {
        queue = new LimitedQueue<>(size);
        notEmpty = new Semaphore(0);
        notFull = new Semaphore(size);
    }

	public void put(final int value) {
        try {
            notFull.acquire();
        } catch (final InterruptedException e) {
            e.printStackTrace();
        }
        synchronized (queue) {
            queue.add(value);
        }
        notEmpty.release();
	}

	public int get() {
        Integer result;
        try {
            notEmpty.acquire();
        } catch (final InterruptedException e) {
            e.printStackTrace();
        }
        synchronized (queue) {
            result = queue.poll();
        }
        notFull.release();
        if (result == null) {
            return -1;
        }
        return result;
	}
}
