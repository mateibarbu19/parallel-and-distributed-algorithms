package bugConcurrentHashMap;

import java.util.concurrent.ConcurrentHashMap;

public class MyThread implements Runnable {
	public static ConcurrentHashMap<Integer, Integer> hashMap = new ConcurrentHashMap<>();
	private final int id;

	public MyThread(final int id) {
		this.id = id;
	}

	private void addValue(final int key, final int value) {
		Integer oldVal = hashMap.putIfAbsent(key, value);
		if (oldVal != null) {
			hashMap.replace(key, oldVal + value);
		}
	}
	
	@Override
	public void run() {
		for (int i = 0; i < Main.N; i++) {
			addValue(i, (2 + id) * i);
		}
	}
}
