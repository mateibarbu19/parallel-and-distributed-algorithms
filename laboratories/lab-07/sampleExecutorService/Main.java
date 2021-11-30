package sampleExecutorService;

import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.atomic.AtomicInteger;

public class Main {

	public static void main(final String[] args) {
		final AtomicInteger inQueue = new AtomicInteger(0);
		final ExecutorService tpe = Executors.newFixedThreadPool(4);

		inQueue.incrementAndGet();
		tpe.submit(new MyRunnable("files", tpe, inQueue));
	}
}
