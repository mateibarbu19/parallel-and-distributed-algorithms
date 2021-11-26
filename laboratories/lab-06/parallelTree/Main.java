package parallelTree;

import java.util.concurrent.Semaphore;

public class Main {
	static int N_ITERATIONS = 100;
    public final static int NUMBER_OF_THREADS = 3;
	public final static Semaphore noVerify = new Semaphore(0);

	public static void main(final String[] args) {
		final Thread[] threads = new Thread[3];
		System.out.println("Parallel tree problem.");
		
		for (int j = 0; j < N_ITERATIONS; j++) {
			final TreeNode tree = new TreeNode(1);
			threads[0] = new Thread(new ReadTreePart(tree, "treePart1.txt"));
			threads[1] = new Thread(new ReadTreePart(tree, "treePart2.txt"));
			threads[2] = new Thread(new VerifyTree(tree));
			for (int i = 0; i < 3; i++) {
				threads[i].start();
			}

			for (int i = 0; i < 3; i++) {
				try {
					threads[i].join();
				} catch (final InterruptedException e) {
					e.printStackTrace();
				}
			}
		}
	}
}