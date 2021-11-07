package shortestPathsFloyd_Warshall;

import java.util.concurrent.CyclicBarrier;

public class Main {
    private static void printSquareMatrix(int[][] matrix, int N) {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                System.out.printf("%2d ",matrix[i][j]);
            }
            System.out.println();
        }
    }

    private static void sequentialSolver(int[][] graph, int N) {
        for (int k = 0; k < N; k++) {
            for (int i = 0; i < N; i++) {
                for (int j = 0; j < N; j++) {
                    graph[i][j] = Math.min(graph[i][k] + graph[k][j], graph[i][j]);
                }
            }
        }
    }

    public static void main(String[] args) {
        final int inf = Integer.MAX_VALUE / 3;
        int N = 5;
        int P = 2;
        // Integer.min(Runtime.getRuntime().availableProcessors(), N);
        Thread[] threads = new Thread[P];

        int[][] graph = {
                {  0,   3,   8, inf,  -4},
                {inf,   0, inf,   1,   7},
                {inf,   4,   0, inf,   1},
                {  2, inf,  -5,   0, inf},
                {inf, inf, inf,   6,   0}};

        int[][] parGraph = new int[N][N];
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                parGraph[i][j] = graph[i][j];
            }
        }

        CyclicBarrier barrier = new CyclicBarrier(P);

        for (int i = 0; i < P; i++) {
            threads[i] = new MyThread(parGraph, i, N, P, barrier);
            threads[i].start();
        }

        for (int i = 0; i < P; i++) {
            try {
                threads[i].join();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }

        sequentialSolver(graph, 5);
        printSquareMatrix(graph, 5);
        System.out.println();
        printSquareMatrix(parGraph, 5);
    }
}
