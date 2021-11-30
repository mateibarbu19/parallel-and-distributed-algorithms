package sampleExecutorServiceCompletableFuture;

import java.io.File;
import java.util.concurrent.CompletableFuture;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.atomic.AtomicInteger;

public class MyRunnable implements Runnable {
	private final ExecutorService tpe;
	private final String path;
	private final String filename;
	private final AtomicInteger counter;
	private final CompletableFuture<String> completableFuture;

	public MyRunnable(
			final ExecutorService tpe,
			final String path,
			final String filename,
			final AtomicInteger counter, final CompletableFuture<String> completableFuture) {
		this.tpe = tpe;
		this.path = path;
		this.filename = filename;
		this.counter = counter;
		this.completableFuture = completableFuture;
	}

	@Override
	public void run() {
		final File file = new File(path);
		if (file.isFile()) {
			if (file.getName().equals(filename)) {
				completableFuture.complete(file.getAbsolutePath());
				tpe.shutdown();
			}
		} else if (file.isDirectory()) {
			final var files = file.listFiles();
			if (files != null) {
				for (final var f : files) {
					counter.incrementAndGet();
					final Runnable t = new MyRunnable(tpe, f.getPath(), filename, counter, completableFuture);
					tpe.submit(t);
				}
			}
		}

		final int left = counter.decrementAndGet();
		if (left == 0) {
			completableFuture.complete(null);
			tpe.shutdown();
		}
	}
}
