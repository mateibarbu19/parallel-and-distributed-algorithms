package bugConcurrentHashMap;

import java.util.Map.Entry;
import java.util.concurrent.ConcurrentHashMap;

/**
 *         DO NOT MODIFY
 */
public class Main {
	public final static int N = 100000;
	private final static int N_ITERATIONS = 100;

	public static void main(final String[] args) {
		final Thread[] threads = new Thread[2];
		boolean sw = true;
		System.out.println("Concurrent HashMap with a Bug.");
		
		MyThread.hashMap = new ConcurrentHashMap<>();
		for (int j = 0; j < N_ITERATIONS; j++) {
			MyThread.hashMap.clear();

			for (int i = 0; i < 2; i++) {
				threads[i] = new Thread(new MyThread(i));
				threads[i].start();
			}

			for (int i = 0; i < 2; i++) {
				try {
					threads[i].join();
				} catch (final InterruptedException e) {
					e.printStackTrace();
				}
			}
			
			for (final Entry<Integer, Integer> entry : MyThread.hashMap.entrySet()) {
				if(entry.getKey() * 5 != entry.getValue()) {
					System.out.println("key " + entry.getKey() + " has incorrect value " + entry.getValue());
					sw = false;
				}
			}
		}
		if (sw) {
			System.out.println("Correct");
		}
	}
}
