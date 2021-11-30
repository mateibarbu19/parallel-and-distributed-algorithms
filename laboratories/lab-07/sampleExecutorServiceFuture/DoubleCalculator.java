package sampleExecutorServiceFuture;

import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;

public class DoubleCalculator {
    private final ExecutorService executorService = Executors.newCachedThreadPool();

    public Future<Integer> calculate(final int input) {
        return executorService.submit(() -> {
            Thread.sleep(1000);
            return 2 * input;
        });
    }

    public void shutdown() {
        executorService.shutdown();
    }
}