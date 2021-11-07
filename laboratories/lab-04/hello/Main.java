package hello;

public class Main {
    public static void main(String[] args) {
        int processors = Runtime.getRuntime().availableProcessors();
        System.out.println("Running process on " + processors + " threads.");

        Thread[] threads = new Thread[processors];

        for (int i = 0; i < processors; i++) {
            threads[i] = new Thread(new MyThread(i));
            threads[i].start();
		}
		for (int i = 0; i < processors; i++) {
            try {
                threads[i].join();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }
}
