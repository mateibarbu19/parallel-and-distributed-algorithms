package multipleProducersMultipleConsumers;
/**
 *         DO NOT MODIFY
 */
public class Producer implements Runnable {
	Buffer buffer;
	int id;

	Producer(final Buffer buffer, final int id) {
		this.buffer = buffer;
		this.id = id;
	}

	@Override
	public void run() {
		for (int i = 0; i < Main.N; i++) {
			// System.out.println(i);
			buffer.put(i);
		}
		System.out.println("Producer " + id + " finished.");
	}

}
/* DO NOT MODIFY */