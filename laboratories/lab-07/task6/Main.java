package task6;

import java.util.concurrent.ForkJoinPool;

public class Main {
    public static int N = 4;

    public static void main(final String[] args) {
        final ForkJoinPool fjp = new ForkJoinPool(Runtime.getRuntime().availableProcessors());
        final int[] graph = new int[N];
        final Boolean success = fjp.invoke(new QueenTask(0, N, graph));
        fjp.shutdown();

        while (!fjp.isTerminated()) {
            try {
                Thread.sleep(500);
            } catch (final InterruptedException e) {
                System.err.println("Main thread was interrupted!");
                e.printStackTrace();
            }
        }

        if (success) {
            System.out.println("The path finding was successful.");
        } else {
            System.out.println("The path finding was not successful");
        }
    }
}
