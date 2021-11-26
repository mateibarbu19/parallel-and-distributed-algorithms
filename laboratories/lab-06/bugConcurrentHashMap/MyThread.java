package bugConcurrentHashMap;

import java.util.concurrent.ConcurrentHashMap;

public class MyThread implements Runnable {
	public static ConcurrentHashMap<Integer, Integer> hashMap = new ConcurrentHashMap<>();
	private final int id;

	public MyThread(final int id) {
		this.id = id;
	}

	private void addValue(final int key, final int value) {
		final Integer oldVal = hashMap.putIfAbsent(key, value);
		if (oldVal != null) {
			hashMap.replace(key, oldVal + value);
		}
		// Alternative
		// hashMap.putIfAbsent(key, 0);
		// hashMap.computeIfPresent(key, (k, v) -> value + v);
	}
	
	@Override
	public void run() {
		for (int i = 0; i < Main.N; i++) {
			addValue(i, (2 + id) * i);
		}
	}
}
