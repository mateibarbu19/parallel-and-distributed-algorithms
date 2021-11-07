package shortestPathsFloyd_Warshall;

import java.util.concurrent.BrokenBarrierException;
import java.util.concurrent.CyclicBarrier;

public class MyThread extends Thread {
    private final int[][] d;
    private final int id;
    private final int N;
    private final int P;
    private final CyclicBarrier barrier;

    public MyThread(int[][] d, int id, int n, int p, CyclicBarrier barrier) {
        this.d = d;
        this.id = id;
        N = n;
        P = p;
        this.barrier = barrier;
    }

    @Override
    public void run() {
        int start = id * N / P;
        int end = (id + 1) * N / P;
        for (int k = 0; k < N; k++) {
            for (int i = start; i < end; i++) {
                if (i != k) {
                    for (int j = 0; j < N; j++) {
                        d[i][j] = Integer.min(d[i][j], d[i][k] + d[k][j]);
                    }
                }
            }

            try {
                barrier.await();
            } catch (InterruptedException | BrokenBarrierException e) {
                e.printStackTrace();
            }

            if (start < k && k < end) {
                for (int j = 0; j < N; j++) {
                    d[k][j] = Integer.min(d[k][j], d[k][k] + d[k][j]);
                }
            }
    
            try {
                barrier.await();
            } catch (InterruptedException | BrokenBarrierException e) {
                e.printStackTrace();
            }
        }
    }
}