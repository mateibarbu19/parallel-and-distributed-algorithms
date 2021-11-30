package sampleExecutorService;

import java.io.File;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.atomic.AtomicInteger;

public class MyRunnable implements Runnable {
	private final String path;
	private final ExecutorService tpe;
	private final AtomicInteger inQueue;

	public MyRunnable(final String path, final ExecutorService tpe, final AtomicInteger inQueue) {
		this.path = path;
		this.tpe = tpe;
		this.inQueue = inQueue;
	}

	@Override
	public void run() {
		final File file = new File(path);
		if (file.isFile()) {
			System.out.println(file.getPath());
		} else if (file.isDirectory()) {
			final File[] files = file.listFiles();

			if (files != null) {
				for (final File f : files) {
					inQueue.incrementAndGet();
					tpe.submit(new MyRunnable(f.getPath(), tpe, inQueue));
				}
			}
		}

		final int left = inQueue.decrementAndGet();
		if (left == 0) {
			tpe.shutdown();
		}
	}
}
