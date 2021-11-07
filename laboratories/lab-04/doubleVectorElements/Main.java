package doubleVectorElements;

public class Main {
    public static void main(String[] args) {
        int N = 100000013;
        int[] v = new int[N];
        int P = Integer.min(Runtime.getRuntime().availableProcessors(), N);
        Thread[] threads = new Thread[P];

        for (int i = 0; i < N; i++) {
            v[i] = i;
        }
        for (int i = 0; i < P; i++) {
            threads[i] = new MyThread(v, i * N / P, (i + 1) * N / P);
            threads[i].start();
        }

		for (int i = 0; i < P; i++) {
            try {
                threads[i].join();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }

        for (int i = 0; i < N; i++) {
            if (v[i] != i * 2) {
                System.out.println("Wrong answer!");
                System.exit(1);
            }
        }
        System.out.println("Correct.");
    }

}
