package multipleProducersMultipleConsumers;

import java.util.concurrent.BlockingQueue;
import java.util.concurrent.LinkedBlockingQueue;

public class Buffer {
	private final BlockingQueue<Integer> queue;
	int value;

	Buffer(final int capacity) {
		queue = new LinkedBlockingQueue<>(capacity);
	}

	void put(final int value) {
		try {
			queue.put(value);
		} catch (final InterruptedException e) {
			System.err.println("Buffer: Could not put an element!");
			e.printStackTrace();
		}
	}

	int get() {
		Integer value = null;

		try {
			value = queue.take();
		} catch (final InterruptedException e) {
			System.err.println("Buffer: Could not get an element!");
			e.printStackTrace();
		}

		return value;
	}
}
