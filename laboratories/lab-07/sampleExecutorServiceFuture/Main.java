package sampleExecutorServiceFuture;

import java.util.concurrent.ExecutionException;
import java.util.concurrent.Future;

public class Main {
    public static void main(final String[] args) throws ExecutionException, InterruptedException {
        final DoubleCalculator doubleCalculator = new DoubleCalculator();
        final Future<Integer> future = doubleCalculator.calculate(10);

        while (!future.isDone()) {
            System.out.println("Calculating...");
            Thread.sleep(300);
        }

        final Integer result = future.get();
        System.out.println(result);

        doubleCalculator.shutdown();
    }
}