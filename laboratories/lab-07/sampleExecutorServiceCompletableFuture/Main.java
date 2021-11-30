package sampleExecutorServiceCompletableFuture;

import java.util.concurrent.CompletableFuture;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.atomic.AtomicInteger;

public class Main {

	public static void main(final String[] args) throws ExecutionException, InterruptedException {
		final ExecutorService tpe = Executors.newFixedThreadPool(4);
		final CompletableFuture<String> completableFuture = new CompletableFuture<>();
		final AtomicInteger counter = new AtomicInteger(0);
		counter.incrementAndGet();
		tpe.submit(new MyRunnable(tpe, "files", "somefile.txt", counter, completableFuture));

		final var result = completableFuture.get();
		if (result != null) {
			System.out.println("File was found at this path: " + result);
		} else {
			System.out.println("File was not found.");
		}
	}
}
